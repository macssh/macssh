short transBuffer(short oldtable, short newtable);
BytePtr		GetTranslationResource(short id);
BytePtr		ReturnTablePtr(short table, Boolean out);
void	Setup_Default_Tables(void);
void trInit(MenuHandle whichMenu);
unsigned char	ftp_iso_mac(unsigned char *ascii);
void	trbuf_nat_mac(unsigned char *buf, short len, short table);
void	trbuf_ftp_mac(unsigned char *buf, short len);
unsigned char	mac_nat(unsigned char *ascii, short table);
unsigned char	ftp_mac_iso(unsigned char *ascii);
void	trbuf_mac_nat(unsigned char *buf, short len, short table);
void	trbuf_mac_ftp(unsigned char *buf, short len);

