
/* parse.c */

void	kbflush(struct WindRec *tw);
void	kbwrite(struct WindRec *tw, unsigned char *string, short len);
void	SendStringAsIfTyped(struct WindRec *tw, char *string, short len);
void	SendCRAsIfTyped(struct WindRec *tw);
void	Parseunload(void);
void	parse(struct WindRec *tw, unsigned char *st, short cnt);
void	rlogin_parse(struct WindRec *, unsigned char *, short);
void	SendNAWSinfo(WindRec *s, short horiz, short vert);
void	telnet_send_initial_options(WindRec *tw);
void	send_do(short port, short option);
void	send_dont(short port, short option);
void	otpauto(struct WindRec *, char *, short);
void	handlewait(struct WindRec *, char *, short);
void	handleread(struct WindRec *tw, char *string, short len);
