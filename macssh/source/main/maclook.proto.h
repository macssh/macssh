typedef struct WindRec WindRec, * WindRecPtr;
/* maclook.c */
void MiscUnload(void);
short detachGraphics(short dnum);
short WindowPtr2ScreenIndex(WindowPtr wn);
WindRecPtr	WindowPtr2WindRecPtr(WindowPtr wn);
short findbyVS(short vs);
void setgraphcurs(void);
void unsetgraphcurs(void);
short VGalive(short dnum);
void destroyGraphics(short dnum);
short FindByVG(short vg);
void TekEnable(short vg);
void TekDisable(short vg);
void showNetNumbers(void);
void setupkeys(void);
