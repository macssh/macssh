// binsubs.c
// MacBinary Subroutines.

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "maclook.proto.h"
#include "bkgr.proto.h"
#include "MacBinary.h"
#include "telneterrors.h"
#include "binsubs.proto.h"
#include "translate.proto.h"
#include "telnetdebug.h"
#include "DlogUtils.proto.h"	// For WriteZero

static	unsigned short	CalculateCRC(unsigned char *ptr, short count, unsigned short crc);
static	void SetFileInfo(short volume, long dirID, StringPtr name, HFileParam *iop);
static	void MakeTextFile(short volume, long dirID, StringPtr name, HFileParam *iop);
static	short isMacBinary(MBHead *p);
static	OSErr bwrite( MBFile *out, char *buffer, long size);
static	void ProcessMBHead (MBFile *out, MBHead *header);

extern	MBFile		/* BYU */
	*mbfp;			/* BYU */

#define BLOCKS(x)	((x+127)/128)
/*#define lmove(f,t)	memmove(f,t,(size_t) 4)		/* BYU LSC */

MBHead	*mbh;
char	buffer[128];

unsigned short	CalculateCRC(unsigned char *ptr, short count, unsigned short crc)
{
	unsigned short	i;
	
	crc = 0;
	while (count -- > 0) {
		crc = crc ^ (unsigned short)*ptr++ << 8;
		for (i = 0; i < 8; i++)
			if (crc & 0x8000)
				crc = crc << 1 ^ 0x1021;
			else
				crc = crc << 1;
	}
	
	return crc;
}

void GetFileInfo(short volume, long dirID, StringPtr name, HFileParam *iop)
{
	OSErr err;
	iop->ioNamePtr = name;
	iop->ioVRefNum = volume;
	iop->ioDirID = dirID;
	iop->ioFVersNum = 0;
	iop->ioFDirIndex = 0;
	err = PBHGetFInfoSync((HParmBlkPtr) iop);
}

void SetFileInfo(short volume, long dirID, StringPtr name, HFileParam *iop)
{
	OSErr err;
	iop->ioNamePtr = name;
	iop->ioVRefNum = volume;
	iop->ioDirID = dirID;
	iop->ioFVersNum = 0;
	iop->ioFDirIndex = 0;
	err = PBHSetFInfoSync((HParmBlkPtr) iop);
}

void MakeTextFile(short volume, long dirID, StringPtr name, HFileParam *iop)
{
	GetFileInfo(volume, dirID, name, iop);
	iop->ioFlFndrInfo.fdType = 'TEXT';
	iop->ioFlFndrInfo.fdCreator = gFTPServerPrefs->TextCreator;
	SetFileInfo(volume, dirID, name, iop);
}

short isMacBinary(MBHead *p)
{
	unsigned short	crc;
	
	if ((p->nlen > 0)   &&	(p->nlen < 65)  &&
		(p->zero1 == 0) &&	(p->zero2 == 0) && (p->zero3 == 0))	{	// Assume MB I
			crc = CalculateCRC((unsigned char *)p, 124, 0);
			if (((short)p->crc == crc) && (p->mb2versnum > 128))	{		// Check for MB II
				if (p->mb2minvers > 129)
					return(0);			// If vers is greater than 129, leave it alone
				
				return (1);				// Valid MB II file.
			}
			else {			
				p->flags2 = 0;			// So we can use same routines for MB I & II
				return (1);				// Valid MB I file (we make it a II file on the fly.)
				}
		}

	return(0);							// Not a Macbinary file			
}

long MBsize( MBFile *mbfp)
{
	long size = 0;
	OSErr ret;
	
	ret = GetEOF( mbfp->fd, &size );			/* length of file data fork */
	if (ret != noErr) 
		size = 0;

	return(size);
}

MBFile *MBopen(char *file, short vrefnum, long dirID, short mode)
{
	MBFile *mbfp;
	OSErr err;
	int i;
	
	if ((mbfp = (MBFile *) myNewPtr(sizeof(MBFile))) == NULL)
		return(NULL);

	if (gFTPServerPrefs->DoISOtranslation)				// MP: translation method
		trbuf_ftp_mac((unsigned char *)file, strlen(file));

	BlockMoveData(file, mbfp->name, 32);
	if (strlen(file) > 31)
		mbfp->name[31] = 0;

	c2pstr((char *)mbfp->name);

	if (mbfp->name[1] == '.') mbfp->name[1] = '_'; //replace leading period with slash

	for (i = 1; i <= mbfp->name[0]; i++)		// replace embedded slashes, colons, and CRs
		if (mbfp->name[i] == '/' || mbfp->name[i] == ':' || mbfp->name[i] == CR) 
			mbfp->name[i] = ' ';

	if (gFTPServerPrefs->DoISOtranslation)
		trbuf_mac_ftp((unsigned char *)file, strlen(file));				// MP: translation method


	mbfp->vrefnum = vrefnum;
	mbfp->dirID = dirID;
	mbfp->mode = mode;

	if ((err = HOpenDF( vrefnum, dirID, mbfp->name, fsRdWrPerm, &mbfp->fd)) != noErr) {
		if ((err==-43) && (mode & MB_WRITE)) {
			err = HCreate( vrefnum, dirID, mbfp->name, gFTPServerPrefs->BinaryCreator,
						gFTPServerPrefs->BinaryType);
			if (mode & MB_ISASCII)	{
				HFileParam blah;
				MakeTextFile(vrefnum, dirID, mbfp->name, &blah);
				}
				
			if (HOpenDF( vrefnum, dirID, mbfp->name, fsRdWrPerm, &mbfp->fd)) 
				return( 0L);
			}
		else 
			return(0L);
		}

	if (mode & MB_APPEND)
		SetFPos(mbfp->fd,fsFromLEOF,0);

	mbfp->binary=0;
	mbfp->sector1=1;
	mbfp->fork=0;
	return( mbfp);
}

OSErr bwrite( MBFile *out, char *buffer, long size)
{
	long len = size;
	OSErr error = noErr;

	if (out->binary) {
		if (out->bytes > 0) {
			if (out->bytes < len) len = out->bytes;
			error= FSWrite( out->fd, &len, buffer);
			out->bytes -= len;
			buffer +=len;
			size -= len;
			}
		if (out->bytes <= 0) {
			if (!out->fork) {
				out->fork = 1;
				out->bytes = BLOCKS(out->rlen)*128;
				SetEOF( out->fd, (long) out->dlen);
				FSClose( out->fd);
				if (out->bytes) {
					HOpenRF( out->vrefnum, out->dirID, out->name, fsRdWrPerm, &out->fd);
					if (size) {
						len = (long) size;
						error= FSWrite( out->fd, &len, buffer);
						}
					}
				else
					out->fd = 0;
				}
			else SetEOF( out->fd, (long) out->rlen);
			}
		}
	else {
		error = FSWrite( out->fd, &len, buffer);
		}
	return (error);
}

void ProcessMBHead (MBFile *out, MBHead *header)
{
	OSErr	err;

	BlockMoveData(header, &out->header, sizeof(MBHead));
	out->binary = 1;
	BlockMoveData(&header->dflen[0], &out->dlen, 4);
	BlockMoveData(&header->rflen[0], &out->rlen, 4);
	out->bytes = BLOCKS(out->dlen)*128;
	out->fork = 0;
	out->sector1 = 0;

	FSClose(out->fd);
	if (HDelete( out->vrefnum, out->dirID, out->name))			/* Error deleting Old File */
		DoError (200 | RESOURCE_ERRORCLASS, LEVEL1, NULL);
		
	BlockMoveData(&out->header.nlen, out->name, 32);

#if 0															/* BYU 2.4.17 */
	MBstat( &out->header.nlen, 1, (long)(BLOCKS(out->dlen)+BLOCKS(out->rlen)) );
#endif															/* BYU 2.4.17 */

	if (out->bytes) {
		if ((err=HOpenDF( out->vrefnum, out->dirID, out->name, fsRdWrPerm, &out->fd)) != noErr) {
			if (err == -43) {
				long	cre,typ;
				
				BlockMoveData(out->header.type, &typ,  4);
				BlockMoveData(out->header.creator, &cre, 4);

				HCreate( out->vrefnum, out->dirID, out->name, cre, typ);
				if (HOpenDF(out->vrefnum, out->dirID, out->name, fsRdWrPerm, &out->fd)) 
					return;
				}
			else {
				return;
				}
			}
		}
	else {
		if ((err=HOpenRF( out->vrefnum, out->dirID, out->name, fsRdWrPerm, &out->fd)) != noErr) {
			if (err == -43) {
				long	cre,typ;

				BlockMoveData(out->header.type, &typ, 4);
				BlockMoveData(out->header.creator, &cre, 4);

				HCreate( out->vrefnum, out->dirID, out->name, cre, typ);
				if (HOpenRF( out->vrefnum, out->dirID, out->name, fsRdWrPerm, &out->fd)) 
					return;
				}
			else {
				return;
				}
			}
		out->fork = 1;
		out->bytes=BLOCKS(out->rlen)*128;
		}
}

long MBwrite(
	MBFile *out,
	void *buffer,	/* BYU LSC */
	long size
  )
{
	long	rsize;
	
	if (size < 1)
		return(0);

	rsize=size;

	if (out->sector1 && (size >= sizeof(struct MBHead)) && (!(out->mode & MB_DISABLE)))  {
		if (isMacBinary((MBHead *) buffer)) {			// WARNING: isMacBinary modifies data
			ProcessMBHead( out, (MBHead *) buffer);
			buffer = (void *)((char *)buffer + 128);
			if ((size-=128) <1)
				return(rsize);
			}
		}

	if (bwrite( out,buffer,size))
		return(-1);
	else
		return( rsize);
}

void MBclose( MBFile *out)
{
	HFileParam finfo;
	long fpos;
	
	if (!out->fd) {
		if (out != NULL) DisposePtr((Ptr)out);
		return;
		}

	if (!(out->mode & MB_DISABLE) && (out->mode & MB_WRITE)) {
		if (out->fork)
			SetEOF( out->fd, (long) out->rlen);
		else
			SetEOF( out->fd, (long) out->dlen);

		FSClose( out->fd);

		GetFileInfo( out->vrefnum, out->dirID, out->name, &finfo);
		BlockMoveData(&out->header.type[0], &finfo.ioFlFndrInfo, sizeof(FInfo));
		BlockMoveData(&out->header.cdate[0], &finfo.ioFlCrDat, 4);
		BlockMoveData(&out->header.mdate[0], &finfo.ioFlMdDat, 4);
		finfo.ioFlFndrInfo.fdFlags &= 0xfeff;
		finfo.ioFlFndrInfo.fdFlags |= (out->header.flags2 & 0x00FF);
		finfo.ioFlRLgLen=out->rlen;
		finfo.ioFlLgLen =out->dlen;
	
		SetFileInfo( out->vrefnum, out->dirID, out->name, &finfo);
		}
	else if (out->mode & MB_WRITE) {
		GetFPos( out->fd, &fpos);
		SetEOF(  out->fd,  fpos);
		FSClose( out->fd);
		}
	else
		FSClose( out->fd);
	
	DisposePtr((Ptr) out);					/* JMB 2.6 -- Nice memory leak, no? */
}

long MBread
  (
	MBFile *in,
	void *buffer,
	long size
  )
{
	char			*p;
	long			rsize=size;
	unsigned short	crc;

	if (in->fork<0) {
		return(-1);
		}

	p = buffer;
	
	if (in->sector1) {
		HFileParam finfo;

//		setmem( &in->header, sizeof(MBHead), 0);
		WriteZero((Ptr)&in->header, sizeof(MBHead));
		BlockMoveData(in->name, &in->header.nlen, (StrLength(in->name) > 31) ? 32 : (StrLength(in->name)+1));
		GetFileInfo( in->vrefnum, in->dirID, in->name, &finfo);
		BlockMoveData(&finfo.ioFlFndrInfo, &in->header.type[0], sizeof(FInfo) );
		in->header.flags2 = finfo.ioFlFndrInfo.fdFlags & 0x00FF;
		in->header.protected = (in->header.zero2 & 0x40)?1:0;
		in->header.zero2 = 0;
		BlockMoveData(&finfo.ioFlLgLen, &in->header.dflen[0], 4);
		BlockMoveData(&finfo.ioFlRLgLen, &in->header.rflen[0], 4);
		BlockMoveData(&finfo.ioFlCrDat, &in->header.cdate[0], 4);
		BlockMoveData(&finfo.ioFlMdDat, &in->header.mdate[0], 4);
		in->header.mb2versnum = 129;
		in->header.mb2minvers = 129;
		crc = CalculateCRC((unsigned char *) &(in->header), 124, 0);
		BlockMoveData(&crc, &(in->header.crc), 2);
		
		in->dlen=finfo.ioFlLgLen;
		in->rlen=finfo.ioFlRLgLen;
		
		if (! (in->mode & MB_DISABLE) ) {
			if (size<128) return(-1);

			BlockMoveData(&in->header, p, 128);
			p +=128;
			size -= 128;
			in->bytes= BLOCKS(in->dlen)*128;
			in->binary=1;
			}
		else {
			in->bytes = in->dlen;
			in->rlen=0;
			in->binary=0;
			}
		in->sector1=0;
#if 0															/* BYU 2.4.17 */
		MBstat( &in->header.nlen, 1, (long) (BLOCKS(in->dlen)+BLOCKS(in->rlen)) );
#endif															/* BYU 2.4.17 */
		}

	if ( size >0) {
		long length = size;
		OSErr err;

		err = FSRead( in->fd, &length, p);

		size -= length;
		in->bytes -=length;
		p += length;

		if (err == -39 || (in->bytes<=0) ) {
			FSClose( in->fd );
			if (in->bytes<0L) in->bytes=0L;
//			setmem(p, in->bytes, 0);			//	Make filler bytes zero
			WriteZero(p, in->bytes);
			size -= in->bytes;
			p    +=      in->bytes;				/* Make adjustments for necessary 128 byte term */
			if (!in->fork ) {
				in->fork=1;
				in->bytes= BLOCKS(in->rlen)*128;
				if (in->bytes) {
					HOpenRF( in->vrefnum, in->dirID, in->name, fsRdWrPerm, &in->fd);
#ifdef READ
					length=(long)size;
					if (length >0L) {
						err = FSRead( in->fd, &length, p);
						size -= length;
						in->bytes -=length;
						}
#endif READ
					}
				else {
					in->fd=0;
					in->fork=-1;					/* Time to close up shop */
					}
				}
			else {
				in->fd=0;
				in->fork=-1;					/* Time to close up shop */
				}
			}
		}
	return( rsize-size); 
}

void init_mb_files(void) {
										// 12/10/97, RJZ.  Make sure ptr isn\'t
										// NULL before dereferencing.
	if (mbfp != NULL)
		mbfp->fd = 0;
}							/* BYU */
							/* BYU */
void close_mb_files(void) {
										// 12/10/97, RJZ.  Make sure ptr isn\'t
										// NULL before dereferencing.
	if (mbfp != NULL && mbfp->fd != 0)
		MBclose( mbfp );
}							/* BYU */
