/***************************************************************************************
	Smart Track Control
	
	SmartTrackControl.c
	
	Copyright ©1994-1996 Bob Bradley, All Rights Reserved
***************************************************************************************/

// Revised 2/98 by Rolf Braun for BetterTelnet 2.0
// to handle signed control minimums and maximums

#ifndef		_SMARTTRACKCONTROL_
	#include	"SmartTrackControl.h"
#endif

#ifndef __APPEARANCE__
#include <Appearance.h>
#endif

extern Boolean gHasAppearance;
extern Boolean gHasAppearance11;

/***************************************************************************************
	SmartTrackControl
	
	Replaces TrackControl to correctly handle situations where the click is in the 
	thumb portion of the control.
***************************************************************************************/

short	SmartTrackControl( ControlRef control, Point where, ControlActionUPP ActionProc )
{
	short					part;
	short					newControlValue;	
	Rect					dragLimits;
	Boolean					isVertical;
	Point					currentPoint;
	short					pixelValue;
	ThemeScrollBarArrowStyle outStyle;


	// Figure out where the click occurred.

	part = TestControl( control, where );

	// Check if TrackControl can handle it on it's own.
	
	if( !( ( part == inThumb ) && ( ActionProc != ( ControlActionUPP ) 0 ) ) )
	{
		// For cases when the click isn't in the thumb AND the action proc is isn't 
		// greater than zero (ie. it's doesn't appears to be a valid ActionProc),
		// TrackControl does it correctly (at least as far as I know).
		
		return( TrackControl( control, where, ActionProc ) );
	}

	// The click is in the indicator (thumb) and the caller passed us a seemingly
	// valid action proc (UniversalProcPtr). We need to replace the behavior of 
	// TrackControl cause TrackControl doesn't handle clicks inThumb correctly. It 
	// doesn't pass anything in the ActionProc's parameter so you'd have to use globals 
	// if you want to implement live scroll. It also would only draw the thumb's outline 
	// which isn't really accurate since you would be changing the data on the fly and 
	// the thumb should be drawn normal and just move with the mouse. This replacement 
	// will do all the work of calculating the thumb position and then calling the 
	// ActionProc but, it will use the same ActionProc parameters as clicks inXXButton 
	// and inPageXX do so it's completely consistant with the other ActionProcs. 
	// This or something which allows the same functionality should have been done from 
	// the start.
	
	// Get the bounds of the control.
	
	GetControlBounds( control, &dragLimits );
		
	// Check it to see if it's a vertical or horizontal control.
	
	if( ( dragLimits.bottom - dragLimits.top ) > ( dragLimits.right - dragLimits.left ) )
	{
		// It's vertical so set the isVertical flag and subtract the arrows from the 
		// top and bottom of the dragLimit rect. We also subtract the area of the thumb 
		// away from the top and bottom. We do this by subtracing half of the thumb size 
		// from the top and half from the bottom. I'm just shifting the bits to right to 
		// divide by two since it's a lot faster to bit-shift than to divide.
		
		isVertical = true;

/* NONO */
#ifndef __powerc
		dragLimits.top 		+= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
		dragLimits.bottom	-= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
#else
		if ( gHasAppearance11
		  && !GetThemeScrollBarArrowStyle( &outStyle )
		  && outStyle == kThemeScrollBarArrowsLowerRight ) {
			dragLimits.top 		+= ( kScrollBarThumbSize >> 1 );
			dragLimits.bottom	-= ( ( 2 * kScrollBarThumbSize ) + ( kScrollBarThumbSize >> 1 ) );
		} else {
			dragLimits.top 		+= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
			dragLimits.bottom	-= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
		}
#endif
/* NONO */

	}
	else
	{
		// It's not vertical so clear the isVertical flag and subtract the arrows from 
		// the left and right of the dragLimit rect. We also subtract the area of the 
		// thumb away from the left and right. We do this by subtracing half of the thumb 
		// size from the left and half from the right. I'm just shifting the bits to right 
		// to divide by two since it's a lot faster to bit-shift than to divide.
		
		isVertical = false;

/* NONO */
#ifndef __powerc
		dragLimits.left 	+= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
		dragLimits.right	-= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
#else
		if ( gHasAppearance11
		  && !GetThemeScrollBarArrowStyle( &outStyle )
		  && outStyle == kThemeScrollBarArrowsLowerRight ) {
			//dragLimits.right	-= 2 * ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
			dragLimits.left 	+= ( kScrollBarThumbSize >> 1 );
			dragLimits.right	-= ( ( 2 * kScrollBarThumbSize ) + ( kScrollBarThumbSize >> 1 ) );
		} else {
			dragLimits.left 	+= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
			dragLimits.right	-= ( kScrollBarThumbSize + ( kScrollBarThumbSize >> 1 ) );
		}
#endif
/* NONO */

	}

	// Start the mouse tracking loop.

	while( StillDown() )
	{
		// Get the current mouse location.
		
		GetMouse( &currentPoint );

		// Since the horizontal and vertical calculations are basically the same, I'll 
		// just explain this once here for both horizontal and vertical.
		//
		// Calculate the new control value based on the mouse's location within the 
		// dragLimits rect of the control. To do this, We pin the current point to a 
		// point inside the dragLimits rect, calculate the distance from the top (or 
		// left) of the dragLimits to the point that we just pinned, and finally, we 
		// translate the value we just calculated from the dragLimits rect's top and 
		// bottom (or left and right) to the controls' minimum and maximum. This tells 
		// us what the control value should be in relation to where the mouse is 
		// located within the dragLimits rect. It's just a transformation function.
		
		if( isVertical )
		{
			currentPoint.v 	= PinValueInRange( currentPoint.v, dragLimits.top, 
											   dragLimits.bottom );
			pixelValue 		= currentPoint.v - dragLimits.top;
			newControlValue = TranslateValue( dragLimits.top, dragLimits.bottom, 
											  pixelValue, GetControlMinimum( control ), 
											  GetControlMaximum( control ) );
		}
		else
		{
			currentPoint.h 	= PinValueInRange( currentPoint.h, dragLimits.left, 
											   dragLimits.right );
			pixelValue 		= currentPoint.h - dragLimits.left;
			newControlValue = TranslateValue( dragLimits.left, dragLimits.right, 
											  pixelValue, GetControlMinimum( control ), 
											  GetControlMaximum( control ) );
		}
		
		// Set the new control value to the value that we just calculated.
		
		SetControlValue( control, newControlValue );
		
		// Call the ActionProc but, this time we call the normal action proc that is 
		// used by inXXButton and inPageXX, not the one TrackControl calls for inThumb.
		
		CallControlActionProc( ActionProc, control, part );
	}
	return( part );
}

/***************************************************************************************
	GetControlBounds
***************************************************************************************/

void	GetControlBounds( ControlRef control, Rect *controlBounds )
{
	*controlBounds = ( **control ).contrlRect;
}

/*************************************************************************************
	PinValueInRange
	
	This pins the value passed in between the start and end parameters. The pinned
	value is return.
*************************************************************************************/

long	PinValueInRange( long value, long rangeStart, long rangeEnd )
{
	// Here's two versions of the same code. The commented one is a lot more readable
	// but, I heard the ? : operators are faster somehow since the compiler can 
	// optimize better of something. It's probably not even true and even if it is, it
	// probably wouldn't make any difference anyway but, I'll let you do the choosing.

// RAB BetterTelnet - I was bored one day, and I tested this. The generated assembly code
// is almost identical, except for the registers it uses. So I guess it doesn't really matter...

/*	if( value > rangeEnd )
	{
		return( rangeEnd );
	}
	else
	{
		if( value < rangeStart )
		{
			return( rangeStart );
		}
		else
		{
			return( value );
		}
	} */


	return( ( value > rangeEnd ) ? rangeEnd : ( ( value < rangeStart ) ? rangeStart : value ) );
}

/*************************************************************************************
	TranslateValue
	
	Translate a value from one range to another. This works in going from larger to
	smaller and from smaller to larger. It is generally used to translate a long
	coordinate to a short coordinate or vice versa in order to work with QuickDraw's
	2-byte limitation on it's coordinate system.
*************************************************************************************/

long	TranslateValue( long min, long max, long value, long newMin, long newMax )
{
	Fixed			ratio;
	long			result;
	long			difference;

	if (newMin < 0) {
		difference = 0 - newMin;
		newMin += difference;
		newMax += difference;
	} else difference = 0;

	ratio 	= FixDiv( ( max - min ), value );
	result	= FixDiv( ( newMax - newMin ), ratio );
	result 	= PinValueInRange( result, newMin, newMax );
	result -= difference;
	return( result );
}
