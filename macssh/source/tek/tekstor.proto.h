TEKSTOREP	newTEKstore(void);
void freeTEKstore(TEKSTOREP s);
short	addTEKstore(TEKSTOREP s, char d);
void	topTEKstore(TEKSTOREP s);
short	nextTEKitem(TEKSTOREP s);
short	TEKunstore(TEKSTOREP s);

#ifdef AARON_CONTORER_HAS_A_BRAIN		// Never true
STOREP newstore(void);
void freestore(STOREP s);
int addstore(STOREP s, char d);
int topstore(STOREP s);
int nextitem(STOREP s);
int unstore(STOREP s);
#endif