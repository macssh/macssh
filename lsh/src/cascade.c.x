/*
CLASS:crypto_cascade_algorithm:crypto_algorithm
*/
#ifndef GABA_DEFINE
struct crypto_cascade_algorithm
{
  struct crypto_algorithm super;
  struct object_list *cascade;
};
extern struct lsh_class crypto_cascade_algorithm_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_crypto_cascade_algorithm_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct crypto_cascade_algorithm *i = (struct crypto_cascade_algorithm *) o;
  mark((struct lsh_object *) i->cascade);
}
struct lsh_class crypto_cascade_algorithm_class =
{
  STATIC_HEADER,
  &(crypto_algorithm_class),
  "crypto_cascade_algorithm",
  sizeof(struct crypto_cascade_algorithm),
  do_crypto_cascade_algorithm_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:crypto_cascade_instance:crypto_instance
*/
#ifndef GABA_DEFINE
struct crypto_cascade_instance
{
  struct crypto_instance super;
  struct object_list *cascade;
};
extern struct lsh_class crypto_cascade_instance_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_crypto_cascade_instance_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct crypto_cascade_instance *i = (struct crypto_cascade_instance *) o;
  mark((struct lsh_object *) i->cascade);
}
struct lsh_class crypto_cascade_instance_class =
{
  STATIC_HEADER,
  &(crypto_instance_class),
  "crypto_cascade_instance",
  sizeof(struct crypto_cascade_instance),
  do_crypto_cascade_instance_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

