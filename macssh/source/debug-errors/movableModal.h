//----------------------------------------------------------------------------------
//	File		: movableModal.h
//	Date		: April 4, 1994
//	Author		: Jim Stout
//	Purpose		: Implements movableModal dialogs
//----------------------------------------------------------------------------------

pascal void movableModalDialog(ModalFilterUPP filter, short *theItem);
short preFilterEvent(DialogPtr d, EventRecord *theEvent);
short doMouseDialog(DialogPtr d, EventRecord *theEvent);
void diskEvent(EventRecord *theEvent);
short CallStdFilterProc(DialogPtr, EventRecord *, short *);
void SetUpMovableModalMenus(void);
void ResetMenus(void);
