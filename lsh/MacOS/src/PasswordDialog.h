/*
	File:		Password.h

	Contains:	This file contains all the interesting password stuff

	Written by: Tim Dierks		

	Copyright:	Copyright © 1991-1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				8/9/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1
				

*/
#include <Dialogs.h>

#define rInternalBufferDialog	256
#define rYesNoDialog			257

#ifdef __cplusplus
extern "C" {
#endif

void InternalBufferDialog (StringPtr inprompt, StringPtr outpassword);
Boolean YesNoDialog (StringPtr inprompt);
OSErr GetScrapHandle( FlavorType dataFlavor, Handle *dataHandle );

#ifdef __cplusplus
}
#endif


