
/* vr.c */
void	ICRunload(void);
void decode0(void);
short decode1(char c);
short VRinit(void);
short VRwrite(char *b, short len);
short VRwindow(union arg av[], char *unused);
void	VRdestroybyName(char *name);
short VRdestroy(union arg av[], char *unused);
short VRmap(union arg av[], char *data);
short VRpixel(union arg av[], char *data);
short VRimp(union arg av[], char *data);
void unimcomp(unsigned char in[], unsigned char out[], short xdim, short xmax);
short unrleit(unsigned char *buf, unsigned char *bufto, short inlen, short outlen);
short VRrle(union arg av[], char *data);
short VRfile(union arg av[], char *unused);
short VRclick(union arg av[], char *unused);
short VRmsave(union arg av[], char *unused);
VRW *VRlookup(char *name);
short VRcleanup(void);
