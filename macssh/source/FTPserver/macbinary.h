#ifdef THINK_C
#pragma options(!align_arrays)	/* BYU 2.4.17 - required for MBHead struct */
#endif

#define MB_READ		0
#define MB_WRITE	1
#define MB_DISABLE	0x80
#define	MB_ISASCII	0x40
#define MB_APPEND	0x20

typedef struct MBHead {		/* MacBinary Header */
	char zero1;				// 000	Old Version Number - Must be zero
	char nlen;				// 001	Filename - Str63
	char name[63];			// 002
	char type[4];			// 065	Filetype
	char creator[4];		// 069	File Creator Type
	char flags;				// 073	Old Finder Flags
	char zero2;				// 074	Must be zero
	char location[6];		// 075	Location information from Finder info
	char protected;			// 081	Low order bit is "protected" bit
	char zero3;				// 082	Must be zero
	char dflen[4];			// 083	Data Fork Length (bytes)
	char rflen[4];			// 087	Resource Fork Length (bytes)
	char cdate[4];			// 091	Creation Date
	char mdate[4];			// 095	Modification Date
	char ginfolen[2];		// 099	MB 1, but unused by Telnet			JMB 2.6
	char flags2;			// 101	Finder Flags, Low Byte				JMB 2.6
	char undefined[14];		// 102	Currently Undefined (9/93)			JMB 2.6
	char unpckdlen[4];		// 116	Unused by Telnet					JMB 2.6
	char sechdlen[2];		// 120	Unused by Telnet					JMB 2.6
	char mb2versnum;		// 122	Uploading Program's version (=129)	JMB 2.6
	char mb2minvers;		// 123	Min version needed (=129)			JMB 2.6
	char crc[2];			// 124	CRC of first 124 bytes				JMB 2.6
	char undefined2[2];		// 126	Currently Undefined (9/93)			JMB 2.6
} MBHead;

typedef struct MBFile {
	Str255 name;			/* The filename */
	short
		fd,					/* Current File Descriptor */
		mode,				/* Are we reading or writing */
		vrefnum,			/* The volume reference number */
		binary,				/* MacBinary active ? */
		sector1,			/* Are we at the first sector of the file */
		fork;				/* 0 if we're writing the DF, 1 if were writing the RF */
	long
		bytes,				/* Length of the current fork */
		rlen,				/* Length of Resource Fork (bytes) */
		dlen,				/* Length of Data Fork (bytes) */
		dirID;				/* The file's dirID */
	MBHead
		header;				/* the first sector... */
} MBFile;
