/*
CLASS:compress_instance:
*/
#ifndef GABA_DEFINE
struct compress_instance
{
  struct lsh_object super;
  struct lsh_string *(*(codec))(struct compress_instance *self,struct lsh_string *data,int free);
};
extern struct lsh_class compress_instance_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class compress_instance_class =
{
  STATIC_HEADER,
  NULL,
  "compress_instance",
  sizeof(struct compress_instance),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:compress_algorithm:
*/
#ifndef GABA_DEFINE
struct compress_algorithm
{
  struct lsh_object super;
  struct compress_instance *(*(make_compress))(struct compress_algorithm *self,int mode);
};
extern struct lsh_class compress_algorithm_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class compress_algorithm_class =
{
  STATIC_HEADER,
  NULL,
  "compress_algorithm",
  sizeof(struct compress_algorithm),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

