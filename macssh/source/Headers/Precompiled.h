/* Precompiled.h */

/* This simply includes the proper pre-compiled header */

#ifdef __cplusplus
/*
 	#ifdef powerc
	 	#include "TelnetHeaderPPC++"
 	#else
 		#include "TelnetHeader68K++"
 	#endif
*/
#include "TelnetHeader.pch"

#else
	#ifdef powerc
  		#include "TelnetHeaderPPC"
	#else
  		#include "TelnetHeader68K"
 	#endif
#endif

