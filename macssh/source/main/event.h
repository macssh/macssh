//event.h 


//This is stuff to translate key codes to VS codes.  There are two tables, for shifted
//and unshifted.  The base is 0x41, the code for the keypad period.  The max is 0x7e, the
//code for the up arrow.  This includes everything but the main portion of the keyboard.

extern unsigned char kpxlate[2][62];
  
  /* ASCII character set defines */

#define DELchar		0x7f	/* BYU LSC - (DEL is defined in MacLook.h) the delete character */
#define	KILLCHAR	0x15	/* the character to kill the local line with */

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if ! defined (__KEYBOARDS__)

enum {
    gestaltKeyboardsLib = FOUR_CHAR_CODE('kbds')     /* Keyboards library */
};

enum
{
    kKeyboardJIS     = 'JIS ',
    kKeyboardANSI    = 'ANSI',
    kKeyboardISO     = 'ISO ',
    kKeyboardUnknown = '????'
};

enum
{
    _KeyboardDispatch = 0xAA7A
};

#pragma import on
extern pascal OSType KBGetLayoutType (short deviceID)
 THREEWORDINLINE(0x303C, 0x0007, _KeyboardDispatch);
#pragma import off

#endif

OSType GetKeyboardLayoutType (short deviceID);

#ifdef __cplusplus
}
#endif
