
/* vrrgmac.c */
void MacRGinit(void);
short MacRGnewwindow(char *name, short x1, short y1, short x2, short y2);
short MacRGsubwindow(short wn);
void MacRGsetwindow(short wn);
void MacRGdestroy(short wn);
void MacRGremove(short wn);
short MacRGfindwind(WindowPtr wind);
void MacRGcopy(WindowPtr wind);
short MacRGupdate(WindowPtr wind);
void MacRGsetdevice(void);
short MacRGraster(char *data, short x1, short y1, short x2, short y2, short rowbytes);
short MacRGfill(short x1, short y1, short x2, short y2);
short MacRGcopyrgn(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4);
short MacRGmap(short start, short length, char *data);
