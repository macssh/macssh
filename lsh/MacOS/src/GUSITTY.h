#ifndef _GUSITTY_
#define _GUSITTY_

#ifdef __cplusplus
extern "C" {
#endif

extern int InstallTTY(int id, int flags);
extern void RemoveTTY(int id, int flags);
extern int ReadCharsFromTTY(int id, int flags, char * buffer, int length);
extern int WriteCharsToTTY(int id, int flags, char * buffer, int length);
extern int AvailableFromTTY(int id, int flags);

#ifdef __cplusplus
}
#endif

#endif /* _GUSITTY_ */
