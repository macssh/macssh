/* IConfig.proto.h */

#include "url.h"	//for TURL defines

void ICinit(void);
OSType GetHelperInfo(TURLKind urlKind);
void stopInternetConfig(void);
void ICUnload(void);
StringPtr aligned_pstring(StringPtr); // Jim Matthews' hack to work around bugs in CW & IC
