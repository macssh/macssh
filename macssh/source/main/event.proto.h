
/* event.c */

#ifdef __cplusplus
extern "C" {
#endif

void	HandleMouseDown(EventRecord myEvent);
void setLastCursor(Cursor *crsr);
short updateCursor(short force);
void NoWindow(void);
Boolean CheckPageKeys(short code);
void SendOneChar(unsigned char sendch);
void HandleKeyDown(EventRecord theEvent,struct WindRec *tw);
void DoEvents(EventRecord*);
void	CloseAWindow(WindowPtr	theWindow);
void HandleRemapping(unsigned char *ascii, Boolean *commanddown,
		Boolean *controldown,Boolean *optiondown, unsigned char *code);//BUGG make it static
void HandleEvent(EventRecord *myEvent);
void NotifyUser (void);
void SetDefaultKCHR(void);
void HandleModeless(EventRecord*, DialogPtr, short);
void CloseModelessDialog(DialogPtr);

#ifdef __cplusplus
}
#endif

