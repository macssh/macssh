
/* Connections.c */
void OpenPortSpecial(MenuHandle menuh, short item);
Boolean PresentOpenConnectionDialog(void);
Boolean OpenConnectionFromURL(char *host, char *portstring, char *user, char *password, short ssh2);
Boolean CreateConnectionFromParams(ConnInitParams **Params);
void CompleteConnectionOpening(short dat, ip_addr the_IP, OSErr DNRerror, char *cname);
void	ConnectionOpenEvent(short port);
void	ConnectionDataEvent(short port);
void	ConnectionFailedEvent(short port);
void	ConnectionClosedEvent(short port);
void destroyport(short wind);
void removeport(WindRecPtr tw);
ConnInitParams	**NameToConnInitParams(StringPtr, Boolean, StringPtr, Boolean *);
// ConnInitParams	**NameToConnInitParam2(StringPtr InputString, Boolean useDefaultTerminal);
// ConnInitParams	**NameToConnInitParam3(StringPtr InputString, Boolean useDefaultTerminal, StringPtr otherPstring);
ConnInitParams	**ReturnDefaultConnInitParams(void);
short numberLiveConnections(void);
short getDefaultPort(short);
short	WindByPort(short port);
WindRec *FindWindByPort(short port);
