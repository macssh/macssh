/*********************************************************************
File		:	GUSI				-	Grand Unified Socket Interface
File		:	GUSIFileTest	-	Test plain files
Author	:	Matthias Neeracher <neeri@iis.ethz.ch>
Language	:	MPW C

$Log$
Revision 1.1.1.1  2001/03/03 21:49:36  chombier
Initial import

Revision 1.4  2000/05/23 06:38:53  neeri
Added descriptor tests

Revision 1.3  2000/01/17 01:38:32  neeri
Fix rename() directory separator handling

Revision 1.2  1999/04/10 04:24:53  neeri
Reimplement ls and ll in a more standard way

Revision 1.1  1998/10/25 11:57:25  neeri
Ready to release 2.0a3

Revision 1.2  1994/12/31  01:10:41  neeri
ANSIfy.
Test FSSpec encoding.

Revision 1.1  1994/02/25  02:46:54  neeri
Initial revision

Revision 0.9  1993/07/29  00:00:00  neeri
scandir

Revision 0.8  1993/07/18  00:00:00  neeri
dirent -> struct dirent

Revision 0.7  1992/12/20  00:00:00  neeri
Allow defaults for choose()

Revision 0.6  1992/12/08  00:00:00  neeri
Pwd()

Revision 0.5  1992/10/27  00:00:00  neeri
Forgot to adapt it to dirent.h

Revision 0.4  1992/09/07  00:00:00  neeri
RdLink()

Revision 0.3  1992/07/25  00:00:00  neeri
Isolated testing gear in GUSITest

Revision 0.2  1992/07/13  00:00:00  neeri
Test choose()

Revision 0.1  1992/06/14  00:00:00  neeri
More tests

*********************************************************************/

#include "GUSITest.h"
#include "GUSIFileSpec.h"
#include <Types.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/unistd.h>
#include <sys/stat.h>

void Stat(char ch1, char ch2, const char * cmd)
{
	int 			res;
	struct stat	statbuf;
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else {
		if (ch2 == 'l') {
			cmd	=	"lstat";
			res 	=	lstat(filename, &statbuf);
		} else {
			cmd 	= 	"stat";
			res 	=	stat(filename, &statbuf);
		}
	
		if (res)	{
			printf("# %s(\"%s\") returned error %s\n", cmd, filename, Explain());
		} else {
			printf("# %s(\"%s\") =\n", cmd, filename);
			DUMP(statbuf.st_dev,d);
			DUMP(statbuf.st_ino,d);
			DUMP(statbuf.st_mode,o);
			DUMP(statbuf.st_nlink,d);
			DUMP(statbuf.st_uid,d);
			DUMP(statbuf.st_gid,d);
			DUMP(statbuf.st_rdev,d);
			DUMP(statbuf.st_size,d);
			DUMP(statbuf.st_atime,u);
			DUMP(statbuf.st_mtime,u);
			DUMP(statbuf.st_ctime,u);
			DUMP(statbuf.st_blksize,d);
			DUMP(statbuf.st_blocks,d);
		}	
		Where();
	}
}

void ChDir(char ch1, char ch2, const char * cmd)
{
	char			directory[80];

	if (sscanf(cmd, "%s", directory) != 1)
		Usage(ch1, ch2);
	else if (chdir(directory))	{
		printf("# chdir(\"%s\") returned error %s\n", directory, Explain());
		Where();
	}
}

void MkDir(char ch1, char ch2, const char * cmd)
{
	char			directory[80];

	if (sscanf(cmd, "%s", directory) != 1)
		Usage(ch1, ch2);
	else if (mkdir(directory))	{
		printf("# mkdir(\"%s\") returned error %s\n", directory, Explain());
		Where();
	}
}

void RmDir(char ch1, char ch2, const char * cmd)
{
	char			directory[80];

	if (sscanf(cmd, "%s", directory) != 1)
		Usage(ch1, ch2);
	else if (rmdir(directory))	{
		printf("# rmdir(\"%s\") returned error %s\n", directory, Explain());
		Where();
	}
}

void List(char ch1, char ch2, const char * cmd)
{
	int					count;
	int					i;
	DIR *					dir;
	struct dirent *	entry;
	char *				dirend;
	char					directory[80];
	struct stat			statbuf;

	if (sscanf(cmd, "%s", directory) != 1)
		strcpy(directory, ":");
	
	if (ch2 == 'l' && !strchr(directory, ':')) {
		directory[0] = ':';
		sscanf(cmd, "%s", directory+1);
	}
	
	if (!(dir = opendir(directory))) {
		printf("# opendir(\"%s\") returned error %s\n", directory, Explain());
		goto error;
	}
	
	printf("# directory \"%s\" =\n", directory);
	
	dirend = directory + strlen(directory);
	if (dirend[-1] != ':')
		*dirend++ = ':';
	
	while (entry = readdir(dir))
		if (ch2 == 's')
			printf("#    %s\n", entry->d_name);
		else {
			strcpy(dirend, entry->d_name);
			
			if (lstat(directory, &statbuf)) 
				printf("# lstat(\"%s\") returned error %s\n", entry->d_name, Explain());
			else
				printf("#    %c %7d %s\n", 
					(statbuf.st_mode & S_IFMT) == S_IFREG ? 'F' :
					(statbuf.st_mode & S_IFMT) == S_IFDIR ? 'D' :
					(statbuf.st_mode & S_IFMT) == S_IFLNK ? 'L' : '?',
					statbuf.st_size,
					entry->d_name);
		}
	closedir(dir);
	
error:
	Where();
}

void Type(char ch1, char ch2, const char * cmd)
{
	FILE * 		fl;
	char			line[500];
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else {
		fl = fopen(filename, "r");
		
		if (!fl)
			printf("# open(\"%s\") returned error %s\n", filename, Explain());
		else {
			printf("# \"%s\" =\n", filename);
			while (fgets(line, 500, fl))
				fputs(line, stdout);

			fclose(fl);
		}
		
		
		Where();
	}
}

void Encode(char ch1, char ch2, const char * cmd)
{
#if 0
	OSErr			err;
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else {
		FSSpec spec;
		
		if (err = Path2FSSpec(filename, &spec))
			fprintf(stderr, "Path2FSSpec(%s) returned error %d\n", filename, err);
		else
			fprintf(stderr, "%s -> %s\n", filename, FSp2Encoding(&spec));
	}
#endif
}

void Glob(char ch1, char ch2, const char * cmd)
{
#if 0
	OSErr			err;
	FileGlobRef	glob;
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else if (!(glob = NewFileGlob(filename)))
		fprintf(stderr, "NewFileGlob(%s) failed.\n", filename);
	else {
		FSSpec spec;
			
		fprintf(stderr, "Glob(%s) matched:\n", filename);
		while (FileGlob2FSSpec(glob, &spec)) {
			fprintf(stderr, "   %s\n", FSp2FullPath(&spec));
			NextFileGlob(glob);
		}
		DisposeFileGlob(glob);
	}
#endif
}

void Edit(char ch1, char ch2, const char * cmd)
{
	FILE * 		fl;
	char			line[500];
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else {
		fl = fopen(filename, "w");
		
		if (!fl)
			printf("# open(\"%s\") returned error %s\n", filename, Explain());
		else	{
			printf("# Enter \"%s\", terminate with \".\"\n", filename);
			while (fgets(line, 500, stdin))
				if (strcmp(line, ".\n"))
					fputs(line, fl);
				else 
					break;
		
			fclose(fl);
		}
	}
}

void Rm(char ch1, char ch2, const char * cmd)
{
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else if (remove(filename))	{
		printf("# remove(\"%s\") returned error %s\n", filename, Explain());
		Where();
	}
}

void Mv(char ch1, char ch2, const char * cmd)
{
	struct stat	statbuf;
	char			oldfilename[80];
	char			newfilename[80];

	if (sscanf(cmd, "%s %s", oldfilename, newfilename) != 2)
		Usage(ch1, ch2);
	else {
		if (!stat(newfilename, &statbuf) && (statbuf.st_mode & S_IFMT) == S_IFDIR) {
			char *	fn;
			char * 	next;
			int 		len 	= 	strlen(newfilename);
			
			/* Extract file name part from oldfilename */
			for (fn = oldfilename; (next = strchr(fn, ':')) && next[1]; fn = next+1);
			
			if (!strchr(newfilename, ':')) {	/* Prepend ':' if necessary */
				newfilename[0] = ':';
				sscanf(cmd, "%s %s", oldfilename, newfilename+1);
				++len;
			}
				
			if (newfilename[len-1] != ':')
				newfilename[len++] = ':';
				
			strcpy(newfilename+len, fn);
		}
		
		if (rename(oldfilename, newfilename))	{
			printf("# rename(\"%s\", \"%s\") returned error %s\n", oldfilename, newfilename, Explain());
			Where();
		}
	}
}

void Link(char ch1, char ch2, const char * cmd)
{
	char			oldfilename[80];
	char			newfilename[80];

	if (sscanf(cmd, "%s %s", oldfilename, newfilename) != 2)
		Usage(ch1, ch2);
	else {		
		if (symlink(oldfilename, newfilename))	{
			printf("# symlink(\"%s\", \"%s\") returned error %s\n", oldfilename, newfilename, Explain());
			Where();
		}
	}
}

void RdLink(char ch1, char ch2, const char * cmd)
{
	char path[200];
	char link[200];
	int  len;

	if (sscanf(cmd, "%s", path) != 1)
		Usage(ch1, ch2);
	
	len = readlink(path, link, 199);
	
	if (len < 0)
		printf("# readlink(\"%s\") returned error %s\n", path, Explain());
	else {
		link[len] = 0;
		printf("# readlink(\"%s\") returned \"%s\"\n", path, link);
	}
		
	Where();
}

void Pwd(char ch1, char ch2, const char * cmd)
{
	char * buf;
	
	buf = getcwd(NULL, 1024);
	
	if (!buf)
		printf("# getcwd() returned error %s\n", Explain());
	else {
		printf("# getcwd() returned \"%s\"\n", buf);
		
		free(buf);
	}
		
	Where();
}

void Access(char ch1, char ch2, const char * cmd)
{
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else {
		if (access(filename, F_OK))
			printf("# access(\"%s\", F_OK) returned error %s\n", filename, Explain());
		else
			printf("# access(\"%s\", F_OK) succeeded.\n", filename, Explain());
		if (access(filename, R_OK))
			printf("# access(\"%s\", R_OK) returned error %s\n", filename, Explain());
		else
			printf("# access(\"%s\", R_OK) succeeded.\n", filename, Explain());
		if (access(filename, W_OK))
			printf("# access(\"%s\", W_OK) returned error %s\n", filename, Explain());
		else
			printf("# access(\"%s\", W_OK) succeeded.\n", filename, Explain());
		if (access(filename, X_OK))
			printf("# access(\"%s\", X_OK) returned error %s\n", filename, Explain());
		else
			printf("# access(\"%s\", X_OK) succeeded.\n", filename, Explain());
		Where();
	}
}

void Open(char ch1, char ch2, const char * cmd)
{
	char			filename[80];

	if (sscanf(cmd, "%s", filename) != 1)
		Usage(ch1, ch2);
	else if ((sock = open(filename, O_RDONLY)) < 0)
		printf("# open(\"%s\") returned error %s\n", filename, Explain());
}

void Seek(char ch1, char ch2, const char * cmd)
{
	int		whence;
	int		offset;
	int		result;
	
	if (sock == -1)	{
		printf("# socket is not open\n");
		Where();
			
		return;
	}

	if (sscanf(cmd, "%d %d", &offset, &whence) != 2) {
		Usage(ch1, ch2);
		return;
	}
	
	if ((result = lseek(sock, offset, whence)) < 0)
		printf("# lseek(%d %d) returned error %s\n", offset, whence, Explain());
	else
		printf("# lseek(%d %d) ended up at offset %d\n", offset, whence, result);
}

main(int argc, char ** argv)
{
	printf("GUSIFileTest		MN 21May00\n\n");

	AddDescriptorCommands();
	
	COMMAND('s', 't', Stat,  "filename", 			"Call stat() on a file");
	COMMAND('s', 'l', Stat,  "filename", 			"Call lstat() on a file");
	COMMAND('c', 'd', ChDir, "directory", 			"Call chdir()");
	COMMAND('l', 's', List,  "[ directory ]", 	"List a directory");
	COMMAND('l', 'l', List,  "[ directory ]", 	"List a directory with more info");
	COMMAND('m', 'd', MkDir, "directory",			"Make a new directory");
	COMMAND('d', 'd', RmDir, "directory",			"Delete an empty directory");
	COMMAND('t', 'y', Type,  "filename",			"Type out the contents of a file");
	COMMAND('e', 'd', Edit,  "filename",			"Enter the contents of a new file");
	COMMAND('m', 'v', Mv,  	 "oldfile newfile",	"Rename/Move a file");
	COMMAND('r', 'm', Rm, 	 "filename",			"Delete a file");
	COMMAND('r', 'l', RdLink,"filename",			"Read symbolic link");
	COMMAND('l', 'n', Link,  "oldfile newfile",	"Create a symbolic link");
	COMMAND('p', 'w', Pwd,	 "",						"Print current directory");
	COMMAND('e', 'n', Encode,"filename",			"Translate filename to encoded FSSpec");
	COMMAND('g', 'l', Glob,	 "filename",			"Expand wildcards");
	COMMAND('a', 'c', Access,"filename",			"Test access rights");
	COMMAND('o', 'p', Open,  "filename",			"Open a file (to test commands on open files)");
	COMMAND('s', 'k', Seek,  "offset whence", 		"Seek on open descriptor");
	RunTest(argc, argv);
}
