                            2  �   	  �    #ifndef GABA_DEFINEstruct ssh_channel{  struct flow_controlled super;  UINT32 channel_number;  struct exception_handler *e;  struct resource_list *resources;  UINT32 rec_window_size;  UINT32 rec_max_packet;  UINT32 send_window_size;  UINT32 send_max_packet;  struct abstract_write *write;  struct alist *request_types;  struct channel_request *request_fallback;  int flags;  int sources;  void (*(receive))(struct ssh_channel *self, int type, struct lsh_string *data);  void (*(send_adjust))(struct ssh_channel *self, UINT32 increment);  void (*(close))(struct ssh_channel *self);  void (*(eof))(struct ssh_channel *self);  struct command_continuation *open_continuation;  struct object_queue pending_requests;  struct object_queue active_requests;};extern struct lsh_class ssh_channel_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstatic void do_ssh_channel_mark(struct lsh_object *o, void (*mark)(struct lsh_object *o)){  struct ssh_channel *i = (struct ssh_channel *) o;  mark((struct lsh_object *) i->e);  mark((struct lsh_object *) i->resources);  mark((struct lsh_object *) i->write);  mark((struct lsh_object *) i->request_types);  mark((struct lsh_object *) i->request_fallback);  mark((struct lsh_object *) i->open_continuation);  object_queue_mark(&i->pending_requests, mark);  object_queue_mark(&i->active_requests, mark);}static void do_ssh_channel_free(struct lsh_object *o){  struct ssh_channel *i = (struct ssh_channel *) o;  object_queue_free(&i->pending_requests);  object_queue_free(&i->active_requests);}struct lsh_class ssh_channel_class ={ STATIC_HEADER,  &flow_controlled_class, "ssh_channel", sizeof(struct ssh_channel),  do_ssh_channel_mark,  do_ssh_channel_free};#endif /* !GABA_DECLARE */#ifndef GABA_DEFINEstruct channel_table{  struct lsh_object super;  struct ssh_channel *(*(channels));  struct alist *global_requests;  struct alist *channel_types;  struct channel_open *open_fallback;  UINT8 (*(in_use));  UINT32 allocated_channels;  UINT32 used_channels;  UINT32 next_channel;  UINT32 channel_count;  UINT32 max_channels;  struct object_queue local_ports;  struct object_queue remote_ports;  struct object_queue active_global_requests;  struct object_queue pending_global_requests;  int pending_close;};extern struct lsh_class channel_table_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstatic void do_channel_table_mark(struct lsh_object *o, void (*mark)(struct lsh_object *o)){  struct channel_table *i = (struct channel_table *) o;  {  unsigned k;  for (k=0; k<i->used_channels; k++)    mark((struct lsh_object *) (i->channels)[k]);  }  mark((struct lsh_object *) i->global_requests);  mark((struct lsh_object *) i->channel_types);  mark((struct lsh_object *) i->open_fallback);  object_queue_mark(&i->local_ports, mark);  object_queue_mark(&i->remote_ports, mark);  object_queue_mark(&i->active_global_requests, mark);  object_queue_mark(&i->pending_global_requests, mark);}static void do_channel_table_free(struct lsh_object *o){  struct channel_table *i = (struct channel_table *) o;  lsh_space_free(i->channels);  lsh_space_free(i->in_use);  object_queue_free(&i->local_ports);  object_queue_free(&i->remote_ports);  object_queue_free(&i->active_global_requests);  object_queue_free(&i->pending_global_requests);}struct lsh_class channel_table_class ={ STATIC_HEADER,  0, "channel_table", sizeof(struct channel_table),  do_channel_table_mark,  do_channel_table_free};#endif /* !GABA_DECLARE */#ifndef GABA_DEFINEstruct global_request{  struct lsh_object super;  void (*(handler))(struct global_request *self, struct ssh_connection *connection, UINT32 type, int want_reply, struct simple_buffer *args, struct command_continuation *c, struct exception_handler *e);};extern struct lsh_class global_request_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstruct lsh_class global_request_class ={ STATIC_HEADER,  0, "global_request", sizeof(struct global_request),  NULL,  NULL};#endif /* !GABA_DECLARE */#ifndef GABA_DEFINEstruct channel_open_exception{  struct exception super;  UINT32 error_code;};extern struct lsh_class channel_open_exception_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstruct lsh_class channel_open_exception_class ={ STATIC_HEADER,  &exception_class, "channel_open_exception", sizeof(struct channel_open_exception),  NULL,  NULL};#endif /* !GABA_DECLARE */#ifndef GABA_DEFINEstruct channel_open{  struct lsh_object super;  void (*(handler))(struct channel_open *self, struct ssh_connection *connection, struct channel_open_info *info, struct simple_buffer *data, struct command_continuation *c, struct exception_handler *e);};extern struct lsh_class channel_open_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstruct lsh_class channel_open_class ={ STATIC_HEADER,  0, "channel_open", sizeof(struct channel_open),  NULL,  NULL};#endif /* !GABA_DECLARE */#ifndef GABA_DEFINEstruct channel_request{  struct lsh_object super;  void (*(handler))(struct channel_request *self, struct ssh_channel *channel, struct ssh_connection *connection, struct channel_request_info *info, struct simple_buffer *args, struct command_continuation *c, struct exception_handler *e);};extern struct lsh_class channel_request_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstruct lsh_class channel_request_class ={ STATIC_HEADER,  0, "channel_request", sizeof(struct channel_request),  NULL,  NULL};#endif /* !GABA_DECLARE */TEXTCWIE R                   