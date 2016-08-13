/*
CLASS:hmac_algorithm:mac_algorithm
*/
#ifndef GABA_DEFINE
struct hmac_algorithm
{
  struct mac_algorithm super;
  struct hash_algorithm *hash;
};
extern struct lsh_class hmac_algorithm_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_hmac_algorithm_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct hmac_algorithm *i = (struct hmac_algorithm *) o;
  mark((struct lsh_object *) i->hash);
}
struct lsh_class hmac_algorithm_class =
{
  STATIC_HEADER,
  &(mac_algorithm_class),
  "hmac_algorithm",
  sizeof(struct hmac_algorithm),
  do_hmac_algorithm_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:hmac_instance:mac_instance
*/
#ifndef GABA_DEFINE
struct hmac_instance
{
  struct mac_instance super;
  struct hash_instance *hinner;
  struct hash_instance *houter;
  struct hash_instance *state;
};
extern struct lsh_class hmac_instance_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_hmac_instance_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct hmac_instance *i = (struct hmac_instance *) o;
  mark((struct lsh_object *) i->hinner);
  mark((struct lsh_object *) i->houter);
  mark((struct lsh_object *) i->state);
}
struct lsh_class hmac_instance_class =
{
  STATIC_HEADER,
  &(mac_instance_class),
  "hmac_instance",
  sizeof(struct hmac_instance),
  do_hmac_instance_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

