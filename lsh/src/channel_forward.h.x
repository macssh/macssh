/*
CLASS:channel_forward:ssh_channel
*/
#ifndef GABA_DEFINE
struct channel_forward
{
  struct ssh_channel super;
  struct lsh_fd *socket;
};
extern struct lsh_class channel_forward_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_channel_forward_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct channel_forward *i = (struct channel_forward *) o;
  mark((struct lsh_object *) i->socket);
}
struct lsh_class channel_forward_class =
{
  STATIC_HEADER,
  &(ssh_channel_class),
  "channel_forward",
  sizeof(struct channel_forward),
  do_channel_forward_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

