
/* vgtek.c */
void	TEKMAINunload(void);
short donothing(void);
short drawc(short vw, short c);
void VGinit(void);
short VGdevice(short vw, short dev);
short VGnewwin(short device, short theVS, short tek4105, short tekclear);
short VGgettektype(short vw);
short VGgettekclear(short vw);
void VGclrstor(short vw);
void VGdumpstore(short vw, short (*func )(short));
void VGdraw(short vw, char c);
void VGpage(short vw);
short VGpred(short vw, short dest);
void VGstopred(short vw);
void VGredraw(short vw, short dest);
void VGgiveinfo(short vw);
void VGzoom(short vw, short x0, short y0, short x1, short y1);
void VGwhatzoom(short vw, short *px0, short *py0, short *px1, short *py1);
void VGzcpy(short src, short dest);
void VGclose(short vw);
short VGwrite(short vw, char *data, short count);
char *VGrgname(short rgdev);
void VGtmode(short rgdev);
void VGgindata(short vw, unsigned short x, unsigned short y, char c, char *a);
short VGgetVS(short theVGnum);
