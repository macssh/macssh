/*
 * :main:authencrypt.c
 */
Boolean auth_encrypt_init(void);
void auth_suboption(tnParams **aedata, unsigned char *subbuffer, long sublength, unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt, Boolean myencrypt, unsigned short port, Boolean forward, char *username);
void auth_encrypt_end(tnParams **aedata);
short encrypt_suboption(tnParams **aedata, unsigned char *subbuffer, long sublength, unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt, Boolean myencrypt);
void kdecrypt(tnParams *tn, unsigned char *buf, long value);
void kencrypt(tnParams *tn, unsigned char *buf, long len);
void qlink(void **flist, void *fentry);
void *qunlink(void **flist, void *fentry);
void	DestroyTickets(void);