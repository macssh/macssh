

short	netputuev
  (
	short	evclass,
	short	what,
	short	data1,
	long	data2
  );

short	netputevent
  (
	short	evclass,
	short	what,
	short	data1,
	long	data2
  );

short	netgetevent(
	short	*evclass,
	short	*data1,
	long	*data2
  );


#ifdef __cplusplus
extern "C" {
#endif

void	ChangeQueueLength(short delta);
void Stask(void);

void DoNetEvents(void);
short setReadBlockSize(short tosize, short scrn);
void pasteText(short scrn);
void FlushNetwork(short scrn);

#ifdef __cplusplus
}
#endif

