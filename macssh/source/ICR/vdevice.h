//vdevice.h

#include "QDOffscreen.h"
#ifndef VDEVS
#define VDEVS
/*
*  Virtual device record.
*  Defines the handles and hooks required for the vdevice code.
*/

//Updated by CCP (2.7) to fix crashing bugs in existance since 2.5
typedef struct {
	GWorldPtr whichWorld;				/* GDevice created off-screen */
	//unsigned char *bp;			/* base pointer of data in the virtual device */
	Rect *bounds;				/* boundary rectangle for vdevice */
} VDevice, *VDevicePtr;


#endif

//#include "vdevice.proto.h"