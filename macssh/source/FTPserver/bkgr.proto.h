void	FTPServerUnload(void);
long Sfread( short pnum, char *buf, long nwant);
long Sfwrite( short pnum, void *buf, long nsrc);
void ftpmess( char *cp);					/* Put a C string on the console */
void setftp( void);
void unsetftp( void);

void ftpd
  (
	short code,
	short curcon
  );

void rftpd
  (
	short code,					/* BYU 2.4.16 */
	short ftpdataport				/* BYU 2.4.16 */
  );

void Sftpstat( long *byt);
void Sftpname( char *s);

ip_addr Sftphost(void);
void Sftpuser( char *user);
void FTPLogUserLoggingIn(void);
Boolean Scheckpass(char *us, char *ps);
Boolean Scompass(char *ps, char *en);
void Sencompass(char *ps, char *en);
void StartUpFTP(void);