/*
CLASS:client_x11_display:
*/
#ifndef GABA_DEFINE
struct client_x11_display
{
  struct lsh_object super;
  socklen_t address_length;
  struct sockaddr (*(address));
  UINT16 screen;
  struct lsh_string *fake;
  struct lsh_string *auth_name;
  struct lsh_string *auth_data;
};
extern struct lsh_class client_x11_display_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_client_x11_display_free(struct lsh_object *o)
{
  struct client_x11_display *i = (struct client_x11_display *) o;
  if ( i->address )
    lsh_space_free(i->address);
  if ( i->fake )
    lsh_string_free(i->fake);
  if ( i->auth_name )
    lsh_string_free(i->auth_name);
  if ( i->auth_data )
    lsh_string_free(i->auth_data);
}
struct lsh_class client_x11_display_class =
{
  STATIC_HEADER,
  NULL,
  "client_x11_display",
  sizeof(struct client_x11_display),
  NULL,
  do_client_x11_display_free,
};
#endif /* !GABA_DECLARE */

/*
CLASS:channel_open_x11:channel_open
*/
#ifndef GABA_DEFINE
struct channel_open_x11
{
  struct channel_open super;
  struct io_backend *backend;
};
extern struct lsh_class channel_open_x11_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_channel_open_x11_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct channel_open_x11 *i = (struct channel_open_x11 *) o;
  mark((struct lsh_object *) i->backend);
}
struct lsh_class channel_open_x11_class =
{
  STATIC_HEADER,
  &(channel_open_class),
  "channel_open_x11",
  sizeof(struct channel_open_x11),
  do_channel_open_x11_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:client_x11_channel:channel_forward
*/
#ifndef GABA_DEFINE
struct client_x11_channel
{
  struct channel_forward super;
  struct client_x11_display *display;
  int state;
  int little_endian;
  unsigned name_length;
  unsigned auth_length;
  UINT32 i;
  struct lsh_string *buffer;
};
extern struct lsh_class client_x11_channel_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_client_x11_channel_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct client_x11_channel *i = (struct client_x11_channel *) o;
  mark((struct lsh_object *) i->display);
}
static void
do_client_x11_channel_free(struct lsh_object *o)
{
  struct client_x11_channel *i = (struct client_x11_channel *) o;
  lsh_string_free(i->buffer);
}
struct lsh_class client_x11_channel_class =
{
  STATIC_HEADER,
  &(channel_forward_class),
  "client_x11_channel",
  sizeof(struct client_x11_channel),
  do_client_x11_channel_mark,
  do_client_x11_channel_free,
};
#endif /* !GABA_DECLARE */

/*
CLASS:channel_open_x11_continuation:command_continuation
*/
#ifndef GABA_DEFINE
struct channel_open_x11_continuation
{
  struct command_continuation super;
  struct client_x11_display *display;
  struct command_continuation *up;
};
extern struct lsh_class channel_open_x11_continuation_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_channel_open_x11_continuation_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct channel_open_x11_continuation *i = (struct channel_open_x11_continuation *) o;
  mark((struct lsh_object *) i->display);
  mark((struct lsh_object *) i->up);
}
struct lsh_class channel_open_x11_continuation_class =
{
  STATIC_HEADER,
  &(command_continuation_class),
  "channel_open_x11_continuation",
  sizeof(struct channel_open_x11_continuation),
  do_channel_open_x11_continuation_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:client_x11_display_resource:resource
*/
#ifndef GABA_DEFINE
struct client_x11_display_resource
{
  struct resource super;
  struct ssh_connection *connection;
  struct client_x11_display *display;
};
extern struct lsh_class client_x11_display_resource_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_client_x11_display_resource_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct client_x11_display_resource *i = (struct client_x11_display_resource *) o;
  mark((struct lsh_object *) i->connection);
  mark((struct lsh_object *) i->display);
}
struct lsh_class client_x11_display_resource_class =
{
  STATIC_HEADER,
  &(resource_class),
  "client_x11_display_resource",
  sizeof(struct client_x11_display_resource),
  do_client_x11_display_resource_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:request_x11_continuation:command_continuation
*/
#ifndef GABA_DEFINE
struct request_x11_continuation
{
  struct command_continuation super;
  struct ssh_connection *connection;
  struct client_x11_display *display;
  struct command_continuation *up;
};
extern struct lsh_class request_x11_continuation_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_request_x11_continuation_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct request_x11_continuation *i = (struct request_x11_continuation *) o;
  mark((struct lsh_object *) i->connection);
  mark((struct lsh_object *) i->display);
  mark((struct lsh_object *) i->up);
}
struct lsh_class request_x11_continuation_class =
{
  STATIC_HEADER,
  &(command_continuation_class),
  "request_x11_continuation",
  sizeof(struct request_x11_continuation),
  do_request_x11_continuation_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:request_x11_forward_command:channel_request_command
*/
#ifndef GABA_DEFINE
struct request_x11_forward_command
{
  struct channel_request_command super;
  struct ssh_connection *connection;
  struct client_x11_display *display;
};
extern struct lsh_class request_x11_forward_command_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_request_x11_forward_command_mark(struct lsh_object *o,
  void (*mark)(struct lsh_object *o))
{
  struct request_x11_forward_command *i = (struct request_x11_forward_command *) o;
  mark((struct lsh_object *) i->connection);
  mark((struct lsh_object *) i->display);
}
struct lsh_class request_x11_forward_command_class =
{
  STATIC_HEADER,
  &(channel_request_command_class),
  "request_x11_forward_command",
  sizeof(struct request_x11_forward_command),
  do_request_x11_forward_command_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

