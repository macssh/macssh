/***************************************************************************************
	Smart Track Control
	
	SmartTrackControl.h
	
	Copyright ©1994-1996 Bob Bradley, All Rights Reserved
***************************************************************************************/

#ifndef		_SMARTTRACKCONTROL_
#define		_SMARTTRACKCONTROL_

/***************************************************************************************
	Constants
***************************************************************************************/

enum
{
	kScrollBarThumbSize = 16
};

/***************************************************************************************
	Prototypes
***************************************************************************************/

short	SmartTrackControl( ControlRef control, Point where, ControlActionUPP ActionProc );
void	GetControlBounds( ControlRef control, Rect *controlBounds );
long	PinValueInRange( long value, long rangeStart, long rangeEnd );
long	TranslateValue( long min, long max, long value, long newMin, long newMax );

#endif
