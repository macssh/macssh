/*
CLASS:packet_debug:abstract_write_pipe
*/
#ifndef GABA_DEFINE
struct packet_debug
{
  struct abstract_write_pipe super;
  struct lsh_string *prefix;
};
extern struct lsh_class packet_debug_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_packet_debug_free(struct lsh_object *o)
{
  struct packet_debug *i = (struct packet_debug *) o;
  lsh_string_free(i->prefix);
}
struct lsh_class packet_debug_class =
{
  STATIC_HEADER,
  &(abstract_write_pipe_class),
  "packet_debug",
  sizeof(struct packet_debug),
  NULL,
  do_packet_debug_free,
};
#endif /* !GABA_DECLARE */

