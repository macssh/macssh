/*
 * :main:authencrypt.c
 */
Boolean auth_encrypt_init(void);
void auth_suboption(tnParams **aedata, unsigned char *subbuffer, long sublength, unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt, Boolean myencrypt, unsigned short port);
void auth_encrypt_end(tnParams **aedata);
short encrypt_suboption(tnParams **aedata, unsigned char *subbuffer, long sublength, unsigned char *sendbuffer, unsigned long *sendlength, char *cname, Boolean hisencrypt, Boolean myencrypt);
unsigned char decrypt(tnParams *tn, long value);
void encrypt(tnParams *tn, unsigned char *buf, long len);
void qlink(void **flist, void *fentry);
void *qunlink(void **flist, void *fentry);
void	DestroyTickets(void);