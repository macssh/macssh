/*
CLASS:packet_compressor:abstract_write_pipe
*/
#ifndef GABA_DEFINE
struct packet_compressor
{
  struct abstract_write_pipe super;
  struct compress_instance *compressor;
  struct ssh_connection *connection;
};
extern struct lsh_class packet_compressor_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_packet_compressor_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct packet_compressor *i = (struct packet_compressor *) o;
  mark((struct lsh_object *) i->compressor);
  mark((struct lsh_object *) i->connection);
}
struct lsh_class packet_compressor_class =
{
  STATIC_HEADER,
  &(abstract_write_pipe_class),
  "packet_compressor",
  sizeof(struct packet_compressor),
  do_packet_compressor_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

