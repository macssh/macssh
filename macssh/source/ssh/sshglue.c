// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

// sshglue.c
// glue code to support secure shell

#define WE_HAVE_SSH 1

#include "wind.h"
#include "sshglue.proto.h"

// prototypes for glued functions
#ifdef WE_HAVE_SSH
void ssh_protocol_initial(WindRec*);
void ssh_packet_read(struct WindRec*, unsigned char*, short);
void ssh_protocol_write(struct WindRec*, unsigned char*, short);
void ssh_protocol_close(WindRec*);
void ssh_randomize(void);
void ssh_exportkey(void);
void ssh_wresize(struct WindRec*);
#endif

void ssh_glue_initial(WindRec* tw)
{
#ifdef WE_HAVE_SSH
	ssh_protocol_initial(tw);
#endif
}

void ssh_glue_read(struct WindRec *tw, unsigned char *st, short cnt)
{
#ifdef WE_HAVE_SSH
	ssh_packet_read(tw, st, cnt);
#endif
}

void ssh_glue_write(struct WindRec *tw, unsigned char *st, short cnt)
{
#ifdef WE_HAVE_SSH
	ssh_protocol_write(tw, st, cnt);
#endif
}

void ssh_glue_close(WindRec* tw)
{
#ifdef WE_HAVE_SSH
	ssh_protocol_close(tw);
#endif
}

void ssh_glue_randomize(void)
{
#ifdef WE_HAVE_SSH
	ssh_randomize();
#endif
}

void ssh_glue_exportkey(void)
{
#ifdef WE_HAVE_SSH
	ssh_exportkey();
#endif
}

void ssh_glue_wresize(struct WindRec* tw)
{
#ifdef WE_HAVE_SSH
	ssh_wresize(tw);
#endif
}

short ssh_glue_installed(void)
{
#ifdef WE_HAVE_SSH
	return 1;
#else
	return 0;
#endif
}
