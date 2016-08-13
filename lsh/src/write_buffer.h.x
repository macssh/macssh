/*
CLASS:flow_controlled:
*/
#ifndef GABA_DEFINE
struct flow_controlled
{
  struct lsh_object super;
  void (*(report))(struct flow_controlled *self,UINT32);
};
extern struct lsh_class flow_controlled_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class flow_controlled_class =
{
  STATIC_HEADER,
  NULL,
  "flow_controlled",
  sizeof(struct flow_controlled),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:write_buffer:abstract_write
*/
#ifndef GABA_DEFINE
struct write_buffer
{
  struct abstract_write super;
  UINT32 block_size;
  UINT8 (*(buffer));
  int empty;
  UINT32 length;
  struct flow_controlled *report;
  int closed;
  struct string_queue q;
  UINT32 pos;
  struct lsh_string *partial;
  UINT32 start;
  UINT32 end;
};
extern struct lsh_class write_buffer_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_write_buffer_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct write_buffer *i = (struct write_buffer *) o;
  mark((struct lsh_object *) i->report);
  string_queue_mark(&(i->q),
    mark);
}
static void
do_write_buffer_free(struct lsh_object *o)
{
  struct write_buffer *i = (struct write_buffer *) o;
  lsh_space_free(i->buffer);
  string_queue_free(&(i->q));
  lsh_string_free(i->partial);
}
struct lsh_class write_buffer_class =
{
  STATIC_HEADER,
  &(abstract_write_class),
  "write_buffer",
  sizeof(struct write_buffer),
  do_write_buffer_mark,
  do_write_buffer_free,
};
#endif /* !GABA_DECLARE */

