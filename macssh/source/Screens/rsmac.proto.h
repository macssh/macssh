#ifndef	__VSDATA__
#include "vsdata.h"
#endif

/* rsmac.c */
void RSinitall(short max);
void RSsetConst(short w);
short RSsetwind(short w);
void RSbell(short w);
Boolean RScursison( short w );
void RScursset( short w, short la, short x, short y );
void RScursoff(short w);
void RScurson(short w, short la, short x, short y);
void RSsetattr(short la, VSAttrib a);
void DoHiliteMode(void);
void RSinvText(short w, Point curr, Point last, RectPtr constrain);
void RSdraw(short w, short x, short y, short la, VSAttrib a, short len, char *ptr);
void RSdelcols(short w, short n);
void RSdelchars(short w, short la, short x, short y, short n);
void RSdellines(short w, short t, short b, short n, short scrolled);
void RSerase(short w, short x1, short y1, short x2, short y2);
void RSinslines(short w, short t, short b, short n, short scrolled);
void RSinscols(short w, short n);
void RSinsstring(short w, short x, short y, short la, VSAttrib a, short len, char *ptr);
void RSmargininfo(short w, short total, short current);
void RSbufinfo(short w, short total, short current, short bottom);
short RSfindscroll(ControlHandle control, short *n);
void RSregnconv(RgnHandle regn, short *x1, short *y1, short *x2, short *y2, short fh, short fw);
Point normalize(Point in_long, short w, Boolean autoScroll);
void	RSsortAnchors(short w);
void RSsetsize(short w, short v, short h, short screenIndex);
void RSbackground(short w, short value);
void RScheckmaxwind(Rect *origRect, short origW, short origH, short *endW, short *endH);
void RSTextFont(short myfnum, short myfsiz, short myface);
