/*********************************************************************
Project	:	GUSI				-	Grand Unified Socket Interface
File		:	GUSITest			-	Testing gear
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:54  chombier
Initial import

Revision 1.1  1998/10/25 11:57:30  neeri
Ready to release 2.0a3

Revision 1.2  1994/12/31  01:16:51  neeri
Add GU·I resource.

Revision 1.1  1994/02/25  02:48:17  neeri
Initial revision

Revision 0.2  1993/03/03  00:00:00  neeri
define GUSI_PREF_VERSION

Revision 0.1  1992/07/13  00:00:00  neeri
Include GUSI.r

*********************************************************************/

#define GUSI_PREF_VERSION '0150'

#include "GUSI.r"

resource 'GU·I' (GUSIRsrcID) {
	text, mpw, noAutoSpin, useChdir, approxStat, 
	noTCPDaemon, noUDPDaemon, 
	noConsole,
	{};
};
