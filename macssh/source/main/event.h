//event.h 


//This is stuff to translate key codes to VS codes.  There are two tables, for shifted
//and unshifted.  The base is 0x41, the code for the keypad period.  The max is 0x7e, the
//code for the up arrow.  This includes everything but the main portion of the keyboard.

extern unsigned char kpxlate[2][62];
  
  /* ASCII character set defines */

#define DELchar		0x7f	/* BYU LSC - (DEL is defined in MacLook.h) the delete character */
#define	KILLCHAR	0x15	/* the character to kill the local line with */
