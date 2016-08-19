/* Public Function */
extern void init (void);

/* Private Functions */

extern void initEvents(void);
extern void initftplog(void);
extern void loadcursor(short CursorID, CursPtr *result);
extern void initmcurs( short debug);
extern void initmac(void);
extern void InitAEHandlers(void);
extern void DoTheGlobalInits(void);
extern void InquireEnvironment( void);
