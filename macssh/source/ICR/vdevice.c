// vdevice.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "vdevice.h"
#include "vdevice.proto.h"
#define PIXEL_DEPTH 8
CGrafPtr origPort;
GDHandle origDev;


int InitVDevice (VDevicePtr whichDevice, PaletteHandle initalPalette)

{
	
	PixMapHandle thePixMap;
	OSErr err;
	CTabHandle theNewColorTable;
	
	GetGWorld(&origPort,&origDev); //save old settings
	
	//first, copy the initalPalette into a color table handle
	
	theNewColorTable = (CTabHandle) myNewHandle(sizeof(ColorTable));
	if (!theNewColorTable)
		return -1;
	Palette2CTab(initalPalette,theNewColorTable);
	
	//init the GWorld
	err = NewGWorld(&whichDevice->whichWorld,PIXEL_DEPTH,whichDevice->bounds,
					theNewColorTable,NULL,0);
	
	if ((!whichDevice->whichWorld)||(err != noErr))
	{
		if (whichDevice->whichWorld)
			DisposeGWorld(whichDevice->whichWorld);
			DisposeHandle((Handle)theNewColorTable);
			return -1;
	}
	SetGWorld(whichDevice->whichWorld,NULL);
	thePixMap = GetGWorldPixMap(whichDevice->whichWorld);
	EraseRect(&(whichDevice->whichWorld->portRect));
	SetGWorld(origPort,origDev);
	return (0);
}
	
	
	
void ColorVDevice
  (
	VDevicePtr vdev,
	PaletteHandle pal
  )
{
	CTabHandle ct;

	ct = (*vdev->whichWorld->portPixMap)->pmTable;		/* handle from vdevice */
	if (!ct)
		return;
	
	Palette2CTab( pal, ct );

	(*ct)->ctSeed = GetCTSeed();					/* give it a unique seed */
	(*ct)->ctFlags = 0x8000;
	
	//MakeITable( ct, (vdev->whichWorld->gdITable, 3 );	/* 3-bit inverse table  */

}

/*******************************************************************************/
/* SetVDevice
*  Set the gdevice and port to our off-screen space.
*  Save the old values for unset.
*/
SetVDevice(VDevicePtr vdev)
{

	GetGWorld(&origPort,&origDev); //save old settings
	if (!vdev->whichWorld)
		return(-1);
	SetGWorld(vdev->whichWorld,NULL);
	return(0);
}

/*******************************************************************************/
/* UnsetVDevice
*  Set the vdevice back to the saved values.
*/
void	UnsetVDevice(void)
{
	SetGWorld(origPort,origDev);
}

/*******************************************************************************/
/* TrashVDevice
*  Get rid of the devices that we created with InitVDevice.
*/
void TrashVDevice(VDevicePtr vdev)
{
	if (vdev->whichWorld)
		DisposeGWorld(vdev->whichWorld);
	return;
}


