                            2  �   	  �    #ifndef GABA_DEFINEstruct blocking_write{  struct abstract_write super;  int fd;  struct exception_handler *e;  const struct exception * (*(write))(int, UINT32, const UINT8 *);};extern struct lsh_class blocking_write_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstatic void do_blocking_write_mark(struct lsh_object *o, void (*mark)(struct lsh_object *o)){  struct blocking_write *i = (struct blocking_write *) o;  mark((struct lsh_object *) i->e);}struct lsh_class blocking_write_class ={ STATIC_HEADER,  &abstract_write_class, "blocking_write", sizeof(struct blocking_write),  do_blocking_write_mark,  NULL};#endif /* !GABA_DECLARE */TEXTCWIE �                   