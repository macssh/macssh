/*
CLASS:terminal_attributes:
*/
#ifndef GABA_DEFINE
struct terminal_attributes
{
  struct lsh_object super;
  struct terminal_attributes *(*(make_raw))(struct terminal_attributes *self);
  struct lsh_string *(*(encode))(struct terminal_attributes *self);
};
extern struct lsh_class terminal_attributes_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class terminal_attributes_class =
{
  STATIC_HEADER,
  NULL,
  "terminal_attributes",
  sizeof(struct terminal_attributes),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:window_change_callback:
*/
#ifndef GABA_DEFINE
struct window_change_callback
{
  struct lsh_object super;
  void (*(f))(struct window_change_callback *self,struct interact *i);
};
extern struct lsh_class window_change_callback_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class window_change_callback_class =
{
  STATIC_HEADER,
  NULL,
  "window_change_callback",
  sizeof(struct window_change_callback),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:interact:
*/
#ifndef GABA_DEFINE
struct interact
{
  struct lsh_object super;
  int (*(is_tty))(struct interact *self);
  struct lsh_string *(*(read_password))(struct interact *self,UINT32 max_length,const struct lsh_string *prompt,int free);
  int (*(yes_or_no))(struct interact *self,const struct lsh_string *prompt,int def,int free);
  struct terminal_attributes *(*(get_attributes))(struct interact *self);
  int (*(set_attributes))(struct interact *self,struct terminal_attributes *attr);
  int (*(window_size))(struct interact *self,struct terminal_dimensions *);
  struct resource *(*(window_change_subscribe))(struct interact *self,struct window_change_callback *callback);
};
extern struct lsh_class interact_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class interact_class =
{
  STATIC_HEADER,
  NULL,
  "interact",
  sizeof(struct interact),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

