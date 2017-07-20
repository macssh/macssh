/* Written for GUSI by Matthias Neeracher <neeri@iis.ee.ethz.ch> */

#ifndef _INTTYPES_H_
#define _INTTYPES_H_

#ifdef __MWERKS__
/* CodeWarrior had this header since at least Pro 5, but I only noticed this now. */
#include <stdint.h>
#else
/*
 * Regrettably, this is needed for *int64_t
 */
#include <MacTypes.h>
#include <ansi_parms.h>

#if __MSL__ < 0x6000
typedef char				int8_t;
#else
typedef signed char			int8_t;
#endif
typedef short				int16_t;
typedef long				int32_t;
typedef SInt64				int64_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef UInt64				uint64_t;
typedef long				intptr_t;
typedef unsigned long		uintptr_t;
#endif

#endif /* _INTTYPES_H_ */
