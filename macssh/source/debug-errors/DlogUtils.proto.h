
/* DlogUtils.c */
void HideDialogItemRange(DialogPtr, short, short);
void ShowDialogItemRange(DialogPtr, short, short);
void DrawBlank(DialogPtr, short);
void GetTEText(DialogPtr dptr, short item, Str255 str);
void SetTEText(DialogPtr dptr, short item, Str255 str);
void Hilite(DialogPtr dptr, short item, short value);
void FlipCheckBox(DialogPtr dptr, short item);
void FlipRadioButton(DialogPtr dptr, short item);
void SetCntrl(DialogPtr dptr, short item, short value);
short GetCntlVal(DialogPtr dptr, short item);
void FlashButton(DialogPtr theDialog, short item);
short findPopupMenuItem(MenuHandle theHandle, Str255 name);
Boolean SelectDirectory(short *VRefNum, long *DirID);
OSErr SetIndString(StringPtr theStr, short resID, short strIndex);
WindowPtr GetNewMyDialog(short template, Ptr wStorage, WindowPtr behind, void (*centerFunction )(short));
WindowPtr GetNewMySmallDialog(short template, Ptr wStorage, WindowPtr behind, void (*centerFunction )(short));
WindowPtr GetNewMySmallStrangeDialog(short template, Ptr wStorage, WindowPtr behind, void (*centerFunction )(short));
WindowPtr GetNewMySmall10StrangeDialog(short template, Ptr wStorage, WindowPtr behind, void (*centerFunction )(short));
void WriteZero(Ptr pointer, long size);
short GetLeading(short fontID, short fontSize);
short GetAscent(short fontID, short fontSize);
void CenterRectIn(Rect *inner, Rect *outer);
void TopCenterRectIn(Rect *inner, Rect *outer);
void BottomCenterRectIn(Rect *inner, Rect *outer);
void ThirdCenterRectIn(Rect *inner, Rect *outer);
void ThirdCenterDialog(short template);
void SecondThirdCenterDialog(short id);
void SecondThirdCenterRectIn(Rect *inner,Rect *outer);

void StandardFileCenter(Point *where, short id);
void pstrcat(StringPtr dst, StringPtr src);
void pstrinsert(StringPtr dst, StringPtr src);
void pstrcpy(StringPtr dst, StringPtr src);
pascal short DLOGwOK( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(DLOGwOK, ModalFilter);
pascal short DLOGwOK_Cancel( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(DLOGwOK_Cancel, ModalFilter);
pascal short DLOGwOK_Cancel_Popup( DialogPtr dptr, EventRecord *evt, short *item);
PROTO_UPP(DLOGwOK_Cancel_Popup, ModalFilter);
pascal short MyDLOGfilter( DialogPtr dptr, EventRecord *evt, short *item, 
							Boolean hasOK, Boolean hasCancel, Boolean hasPopup);
PROTO_UPP(MyDLOGfilter, ModalFilter);
pascal void OutlineItem(DialogPtr dptr, short itm);
PROTO_UPP(OutlineItem, UserItem);
pascal void VersionNumber(DialogPtr dptr, short itm);
PROTO_UPP(VersionNumber, UserItem);

#ifndef NO_UNIVERSAL
void UItemAssign( DialogPtr dptr, short item, UserItemUPP proc);
#else
void UItemAssign( DialogPtr dptr, short item, ProcPtr proc);
#endif
