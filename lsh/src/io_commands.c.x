/*
CLASS:backend_command:command
*/
#ifndef GABA_DEFINE
struct backend_command
{
  struct command super;
  struct io_backend *backend;
};
extern struct lsh_class backend_command_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_backend_command_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct backend_command *i = (struct backend_command *) o;
  mark((struct lsh_object *) i->backend);
}
struct lsh_class backend_command_class =
{
  STATIC_HEADER,
  &(command_class),
  "backend_command",
  sizeof(struct backend_command),
  do_backend_command_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:connect_continuation:command_continuation
*/
#ifndef GABA_DEFINE
struct connect_continuation
{
  struct command_continuation super;
  struct address_info *target;
  struct command_continuation *up;
};
extern struct lsh_class connect_continuation_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_connect_continuation_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct connect_continuation *i = (struct connect_continuation *) o;
  mark((struct lsh_object *) i->target);
  mark((struct lsh_object *) i->up);
}
struct lsh_class connect_continuation_class =
{
  STATIC_HEADER,
  &(command_continuation_class),
  "connect_continuation",
  sizeof(struct connect_continuation),
  do_connect_continuation_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:connect_port:command
*/
#ifndef GABA_DEFINE
struct connect_port
{
  struct command super;
  struct io_backend *backend;
  struct address_info *target;
};
extern struct lsh_class connect_port_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_connect_port_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct connect_port *i = (struct connect_port *) o;
  mark((struct lsh_object *) i->backend);
  mark((struct lsh_object *) i->target);
}
struct lsh_class connect_port_class =
{
  STATIC_HEADER,
  &(command_class),
  "connect_port",
  sizeof(struct connect_port),
  do_connect_port_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:simple_io_command:command
*/
#ifndef GABA_DEFINE
struct simple_io_command
{
  struct command super;
  struct io_backend *backend;
  struct resource_list *resources;
};
extern struct lsh_class simple_io_command_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_simple_io_command_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct simple_io_command *i = (struct simple_io_command *) o;
  mark((struct lsh_object *) i->backend);
  mark((struct lsh_object *) i->resources);
}
struct lsh_class simple_io_command_class =
{
  STATIC_HEADER,
  &(command_class),
  "simple_io_command",
  sizeof(struct simple_io_command),
  do_simple_io_command_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:connect_connection:command
*/
#ifndef GABA_DEFINE
struct connect_connection
{
  struct command super;
  struct io_backend *backend;
};
extern struct lsh_class connect_connection_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_connect_connection_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct connect_connection *i = (struct connect_connection *) o;
  mark((struct lsh_object *) i->backend);
}
struct lsh_class connect_connection_class =
{
  STATIC_HEADER,
  &(command_class),
  "connect_connection",
  sizeof(struct connect_connection),
  do_connect_connection_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:listen_local:command
*/
#ifndef GABA_DEFINE
struct listen_local
{
  struct command super;
  struct io_backend *backend;
  struct local_info *info;
};
extern struct lsh_class listen_local_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_listen_local_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct listen_local *i = (struct listen_local *) o;
  mark((struct lsh_object *) i->backend);
  mark((struct lsh_object *) i->info);
}
struct lsh_class listen_local_class =
{
  STATIC_HEADER,
  &(command_class),
  "listen_local",
  sizeof(struct listen_local),
  do_listen_local_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:connect_local:command
*/
#ifndef GABA_DEFINE
struct connect_local
{
  struct command super;
  struct io_backend *backend;
};
extern struct lsh_class connect_local_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_connect_local_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct connect_local *i = (struct connect_local *) o;
  mark((struct lsh_object *) i->backend);
}
struct lsh_class connect_local_class =
{
  STATIC_HEADER,
  &(command_class),
  "connect_local",
  sizeof(struct connect_local),
  do_connect_local_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

