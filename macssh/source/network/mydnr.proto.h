Boolean DotToNum(BytePtr string,ip_addr *nPtr);
Boolean	TranslateIPtoDNSname(ip_addr ipnum, StringPtr machineName);
OSErr	DoTheDNR(StringPtr hostname, short window);
void	HandleDomainDoneMessage(short screen, long infoptr);
void	HandleDomainDone2Message(short screen, long infoptr);
