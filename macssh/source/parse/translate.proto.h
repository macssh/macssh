
void trInit(MenuHandle whichMenu);


void BuildTranslateMenu(MenuHandle whichMenu);

short GetTranslationIndex(short id);

short transBuffer(short oldtable, short newtable);
BytePtr GetTranslationResource(short id);
BytePtr ReturnTablePtr(short table, Boolean out);
void Setup_Default_Tables(void);

void inittranslation(WindRec *tw);
void disposetranslation(WindRec *tw);

void switchintranslation(WindRec *tw, short national, short charset);
void switchouttranslation(WindRec *tw, short national, short charset);

int trflush_nat_mac(WindRec *tw);
int trflush_mac_nat(WindRec *tw);

int trbuf_nat_mac(WindRec *tw, unsigned char *buf, long *len, unsigned char *out, long *outlen);
int trbuf_mac_nat(WindRec *tw, unsigned char *buf, long *len, unsigned char *out, long *outlen);

Handle htrbuf_nat_mac(WindRec *tw, Handle hin);
Handle htrbuf_mac_nat(WindRec *tw, Handle hin);


unsigned char ftp_mac_iso(unsigned char *ascii);
unsigned char ftp_iso_mac(unsigned char *ascii);

void trbuf_mac_ftp(unsigned char *buf, short len);
void trbuf_ftp_mac(unsigned char *buf, short len);
