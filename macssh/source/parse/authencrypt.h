typedef struct codemodule {
	struct codemodule *next;			/* pointer to next in list */
	module entry;						/* module entry point */
	short npairs;						/* number of type/modifier pairs */
	Boolean encryptok;					/* module does option 38 encryption */
	unsigned short pairs[NTMPAIRS];		/* type/modifier pairs */
	OSType authType;
} codemodule;

