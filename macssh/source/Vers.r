
#define REZ_COMPILER 1

#include "Vers.h"

resource 'vers' (2, purgeable) {
	VMajor, VMinor, VStage, VRelease, verUS,
	VShortString,
	"Secure Shell version 2 client"
};

resource 'vers' (1, purgeable) {
	VMajor, VMinor, VStage, VRelease, verUS,
	VShortString,
	VShortString ", " CopyrightString
};
