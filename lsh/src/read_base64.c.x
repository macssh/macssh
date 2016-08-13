/*
CLASS:read_base64:read_handler
*/
#ifndef GABA_DEFINE
struct read_base64
{
  struct read_handler super;
  struct abstract_write *c;
  struct base64_state state;
  struct lsh_string *buffer;
  UINT32 pos;
};
extern struct lsh_class read_base64_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_read_base64_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct read_base64 *i = (struct read_base64 *) o;
  mark((struct lsh_object *) i->c);
}
static void
do_read_base64_free(struct lsh_object *o)
{
  struct read_base64 *i = (struct read_base64 *) o;
  lsh_string_free(i->buffer);
}
struct lsh_class read_base64_class =
{
  STATIC_HEADER,
  &(read_handler_class),
  "read_base64",
  sizeof(struct read_base64),
  do_read_base64_mark,
  do_read_base64_free,
};
#endif /* !GABA_DECLARE */

