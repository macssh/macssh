pascal OSErr  MyHandleODoc (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandlePDoc (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon);
pascal OSErr  MyHandleOApp (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon);
pascal OSErr  MyHandleQuit (AppleEvent *theAppleEvent, AppleEvent *reply, long
														handlerRefCon);
pascal OSErr  MyHandleGURL (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleSendData (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleSendCR (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleConnect (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleWait (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleRead (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleSuspend (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
pascal OSErr  MyHandleUnSuspend (AppleEvent *theAppleEvent, AppleEvent* reply, long
														handlerRefCon);
OSErr MyGotRequiredParams (AppleEvent *theAppleEvent);

PROTO_UPP(MyHandleODoc, AEEventHandler);
PROTO_UPP(MyHandleOApp, AEEventHandler);
PROTO_UPP(MyHandlePDoc, AEEventHandler);
PROTO_UPP(MyHandleQuit, AEEventHandler);
PROTO_UPP(MyHandleGURL, AEEventHandler);
PROTO_UPP(MyHandleSendData, AEEventHandler);
PROTO_UPP(MyHandleSendCR, AEEventHandler);
PROTO_UPP(MyHandleConnect, AEEventHandler);
PROTO_UPP(MyHandleWait, AEEventHandler);
PROTO_UPP(MyHandleRead, AEEventHandler);
PROTO_UPP(MyHandleSuspend, AEEventHandler);
PROTO_UPP(MyHandleUnSuspend, AEEventHandler);

void AEunload(void);
