
/* vsinterf.c */
short VSinit(short max);
short VSiscapturing(short w);
short VSisprinting(short w);
Boolean	VSisvt7bit(short w);
void ClosePrintingFile(short w);
Boolean VSopencapture(short scrn_num, short w);
void VSclosecapture(short w);
void VScapture(unsigned char *ptr, short len);
short VSvalids(short w);
VSscrn *VSwhereis(short i);
void VSIclrbuf(void);
short VSnewscreen(short vtemulation, short maxlines, short screensave, short numLines,
	short maxwid, unsigned long flags);
short VSdestroy(short w);
short VSredraw(short w, short x1, short y1, short x2, short y2);
short VSOredraw(short, short, short, short, short);
short VSwrite(short w, char *ptr, short len);
short VSwritefast(short w, char *ptr, short len);
void VSsetprintmode(short, short);
//short VSclear(short w);
void VSpossendEM(short w, short x, short y, short echo);
void VSpossend(short w, short x, short y, short echo);
char VSkbsend(short w, unsigned char k, short echo, short blah);
//short VSclearall(short w);
short VSreset(short w);
//char *VSgetline(short w, short y);
void VSscrolright(short w, short n);
void VSredrawLine(short w);
void VSscrolleft(short w, short n);
short VSscroltop(short w);
short VSscrolback(short w, short in);
short VSscrolforward(short w, short n);
short VSsetrgn(short w, short x1, short y1, short x2, short y2);
short VSscrolcontrol(short w, short scrolon, short offtop);
void VSbeepcontrol(short, short);
void VSenableblink(short w, Boolean enableBlink);
short VSgetrgn(short w, short *x1, short *y1, short *x2, short *y2);
//short VSsnapshot(short w);
short VSmaxwidth(short w);
VSlinePtr VSIGetLineStart(short w, short y1);
char *VSIstrcopy(char*, short, char*, short, short);
long VSgettext(short w, short x1, short y1, short x2, short y2,
	char *charp, long max, char *EOLS, short table, short clipspaces);
long VSgetattr(short w, short x1, short y1, short x2, short y2,
			VSAttrib *attrp, long max);
short VSgetlines(short w);
short VSgetnumlines(short w);
short VSsetlines(short w, short lines);
short VSOsetlines(short, short);
short VSsetcols(short w, short myWidth);
short VSgetcols(short w);
short VSIgetNextTabDistance(void);
void VSflushwrite(short w);
void VSrealloc(short);
void VSPulseAll(void);
short VSPulseOne(short, short, short, short, short);
short VSOPulseOne(short, short, short, short, short);