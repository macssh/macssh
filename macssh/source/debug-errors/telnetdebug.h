extern void putln(char *DebugString);
extern void putlln(char *DebugString, short len);
extern long dumpln(long base, char *dest, void *src, long len);
void InitDebug(void);
Boolean	DebugKeys(Boolean cmddwn, unsigned char ascii, short s);
void	ShowDebugWindow(void);
void	HideDebugWindow(void);

// Don't undef this if compiling for PPC!!!!!!!! Yeah, sure Audit works w/PPC! ;)
//#define SUPPORT_AUDIT
