
void netfromport			/* next "open" will use this port */
  (
	short port
  );


#ifdef __cplusplus
extern "C" {
#endif

void	SetPortType(short port, short type);
short	GetPortType(short port);
short	netread(short pnum, void *buffer, short n);
short	netwrite(short pnum, void *buffer, short nsend);
short	Rnetwrite(short pnum, void *buffer, short nsend);
short netpush(short pnum);
short netqlen(short pnum);
short netroom(short pnum);
void netgetip(unsigned char *st);
short netgetport(short);
short netest(short pnum);
short netlisten(ip_port serv);
short	netgetftp(short pnum, ip_addr *addr, ip_port *localPort, ip_port *remotePort);
short netxopen( ip_addr machine, ip_port service, short timeout);
short netclose(short pnum);
short	netabort(short pnum);
void Mnetinit( void);
void netshut(void);
short	findbystream(StreamPtr streamPtr);
short netportencryptstate (short port, Ptr edata);
void networkUPPinit(void);
void netUrgent(void);
short checkPortRotation(short);
short getUrgentFlag(short);
short makestream(void);


#ifdef __cplusplus
}
#endif
