
/* printing.c */
void	PrintingUnload(void);
short userabort(void);
void printGraph(short dnum);
void printPages(TPPrPort prPort, THPrint PrPrintRec, Str255 Title, short columns,
				char **charh, short refNum, short useTitle, short theScreen);
void printText(short vs, Str255 Title, short scrn);
void PrintPageSetup(void);
void PrintSelection(void);
THPrint PrintSetupRecord(void);
