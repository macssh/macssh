
/* rgmp.c */
void	TEKMacPicunload(void);
short RGMPnewwin(void);
char *RGMPdevname(void);
void RGMPinit(void);
short RGMPpencolor(short w, short color);
short RGMPclose(short w);
short RGMPpoint(short w, short x, short y);
short RGMPdrawline(short w, short x0, short y0, short x1, short y1);
void RGMPinfo(short w, short v, short a, short b, short c, short d);
void RGMPcharmode(short w, short rotation, short size);
short RGMPsize(Rect *incoming);
void	RGMPdataline(short blah, short blam, short bluh);
