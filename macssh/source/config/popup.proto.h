
/* popup.c */
Boolean PopupMousedown(DialogPtr dptr, EventRecord *event, short *i);
void PopupInit(DialogPtr dptr, popup *popups);
void PopupCleanup(void);
pascal void DrawPopUp(DialogPtr dptr, short item);
