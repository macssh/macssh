/* rsinterf.proto.h */


/* RSnewwindow/VSnewscreen Flags */
enum {
	RSWwrapon			=	0x00000001,
	RSWshowit			=	0x00000002,
	RSWgoaway			=	0x00000004,
	RSWforcesave 		=	0x00000008,
	RSWallowBold		=	0x00000010,
	RSWcolorBold		=	0x00000020,
	RSWignoreBeeps		=	0x00000040,
	RSWrealbold			=	0x00000080,
	RSWoldscrollback	=	0x00000100,
	RSWjumpscroll		=	0x00000200,
	RSWrealBlink		=	0x00000400,
	RSWvt7bit			=	0x00000800,
	RSWHideScroll		=	0x00001000,
	RSWKeepSelection	=	0x00002000
};



void RSunload(void);
void RSselect(short w, Point pt, EventRecord theEvent);
void RSzoom(GrafPtr window, short code, short shifted);
void RSdrawlocker(short w, RgnHandle visRgn);
short RSupdatecontent(GrafPtr wind, RgnHandle updRgn);
short RSupdate(GrafPtr wind);
short RSTextSelected(short w);
void RSskip(short w, Boolean on);
short RSsize(GrafPtr window, long *where, long modifiers);
void RSshow(short w);
void RSresetcolors(short w);
Boolean RSsetcolors(short w, short n, RGBColor *color);
void RSsendstring(short w, char *ptr, short len);
short RSnewwindow(RectPtr wDims, short scrollback, short width, short lines, StringPtr name,
	short fnum, short fsiz, short screenNumber, short bfnum, short bfsiz, short bfstyle,
	short vtemulation, unsigned long flags);
short RSmouseintext(short w, Point myPoint);
void RSkillwindow(short w);
void RSgetcolors(short w, short n, RGBColor *color);
void RShide(short w);
GrafPtr RSgetwindow(short w);
char **RSGetTextSel(short w, short table, short clipspaces);
char **RSGetTextScreen(short w, short table, short clipspaces);
RgnHandle RSGetTextSelRgn(short w);
short RSfindvwind(GrafPtr wind);
void RSdeactivate(short w);
void RSsetcurstype( short w, long cursType );
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
