/* rsinterf.proto.h */

void	RSunload(void);
void RSselect(short w, Point pt, EventRecord theEvent);
void RSzoom(GrafPtr window, short code, short shifted);
void RSdrawlocker(short w, RgnHandle visRgn);
short RSupdatecontent(GrafPtr wind, RgnHandle updRgn);
short RSupdate(GrafPtr wind);
short RSTextSelected(short w);
void RSskip(short w, Boolean on);
short RSsize(GrafPtr window, long *where, long modifiers);
void RSshow(short w);
Boolean RSsetcolor(short w, short n, RGBColor *color);
void RSsendstring(short w, char *ptr, short len);
short RSnewwindow(RectPtr wDims,short scrollback, short width, short lines, StringPtr name, short wrapon, 
	short fnum, short fsiz, short showit, short goaway, short forcesave, short screenNumber, short allowBold,
	short colorBold, short ignoreBeeps, short bfnum, short bfsiz, short bfstyle, short realbold, short oldScrollback, short jump, short);
short RSmouseintext(short w, Point myPoint);
void RSkillwindow(short w);
void RSgetcolor(short w, short n, RGBColor *color);
void RShide(short w);
GrafPtr RSgetwindow(short w);
char **RSGetTextSel(short w, short table);
RgnHandle RSGetTextSelRgn(short w);
short RSfindvwind(GrafPtr wind);
void RSdeactivate(short w);
void RScursblink(short w);
void RScursblinkon(short w);
void RScursblinkoff(short w);
void RScprompt(short w);
void RScalcwsize(short w, short width);
short RSclick(GrafPtr window, EventRecord theEvent);
void RSactivate(short w);
short ClickInContent(Point where, short w);
Point getlocalmouse(GrafPtr wind);
void RSchangefont(short w, short fnum, long fsiz);
void RSchangeboldfont(short w, short fnum);
void RSchangebold(short, short, short, short);
short RSgetfont(short w, short *pfnum, short *pfsiz);
short RSgetboldfont(short w, short *pfnum);
void RSfontmetrics(void);
pascal void ScrollProc(ControlHandle control, short part);
pascal void ActiveScrollProc(ControlHandle control, short part);
void FlashSelection(short w);
Boolean PointInSelection(Point curr, short w);
void UnHiliteSelection(short w);
void HiliteThis(short w, Point begin, Point end);
void RSUpdatePalette(void);
Boolean RSisInFront(short w);
