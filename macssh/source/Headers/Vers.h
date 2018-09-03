#if REZ_COMPILER
#include "Types.r"
#include "SysTypes.r"
#else
#define	development	0x20
#define	alpha 0x40
#define	beta 0x60
#define	final 0x80
#endif

#define VMajor			3
#define VMinor			(1 * 16) + 0
#define VStage			beta
#define VRelease		1
#define VShortString	"3.0b1"

#define	CopyrightString	"©2016-2018 Brendan Shanks"
