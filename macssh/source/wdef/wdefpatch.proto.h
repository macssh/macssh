/*
 * :Krb:wdefpatch.c
 */
extern void PatchWindowWDEF(WindowPtr window, struct WindRec *tw);
extern void RePatchWindowWDEF(WindowPtr window, struct WindRec *tw);
extern Handle GetPatchStuffHandle(WindowPtr window, struct WindRec *tw);
extern pascal long MyWDEFPatch(short varCode, WindowPtr window, short message, long param);

/* 931112, ragge, NADA, KTH */
extern void setupForGrow(WindowPtr window, short hCharInset, short vCharInset, short hCharSize, short vCharSize);
extern void cleanupForGrow(WindowPtr window);

void loadWDEF(void);
