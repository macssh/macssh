void GetDirectoryName(short vRefNum, long dirID, Str32 name);
short VolumeNameToRefNum(Str32 volumeName);
void PathNameFromDirID(long dirID, short vRefNum, StringPtr fullPathName);
short ChangeDirectory(long *dirID, short *vRefNum,char *pathname);

short wccheck(char *file, char *template);
char *getFileName(char *spec, CInfoPBRec *finfo);
short MakeDirectory(long *dirID, short *vRefNum,char *pathname);
short RemoveDirectory(long *dirID, short *vRefNum,char *pathname);
