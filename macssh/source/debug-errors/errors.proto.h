
/* errors.c */
void DoError(short code, short level, char *mess1);
char *geterrstring(short code);
void FatalCancelAlert(short messageID, StringPtr DefaultButtonString, short InternalID, short MacID);
void FatalAlert(short messageID, short InternalID, short MacID);
void	OperationFailedAlert(short messageID, short InternalID, short MacID);
void	OutOfMemory(short InternalID);
Boolean AskUserAlert(short messageID, Boolean CancelIsDefault);
void loadErrors(void);