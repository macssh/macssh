void GetFileInfo(short volume, long dirID, StringPtr name, HFileParam *iop);
long MBsize( MBFile *mbfp);
MBFile *MBopen(char *file, short vrefnum, long dirID, short mode);

void init_mb_files(void);
void close_mb_files(void);

long MBread
  (
	MBFile *in,
	void *buffer,
	long size
  );

long MBwrite
  (
	MBFile *out,
	void *buffer,
	long size
  );

void MBclose
  (
	MBFile *out
  );

