#if REZ_COMPILER
#include "Types.r"
#include "SysTypes.r"
#else
#define	development	0x20
#define	alpha 0x40
#define	beta 0x60
#define	final 0x80
#endif

#define VMajor			2
#define VMinor			(2 * 16) + 0
#define VStage			alpha
#define VRelease		1
#define VShortString	"2.2a1-CVS"

#define	CopyrightString	"©2000-2001 J.-P. Stierlin"
