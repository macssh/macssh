/*
CLASS:lookup_verifier:
*/
#ifndef GABA_DEFINE
struct lookup_verifier
{
  struct lsh_object super;
  struct verifier *(*(lookup))(struct lookup_verifier *self,int method,struct lsh_user *user,struct lsh_string *key);
};
extern struct lsh_class lookup_verifier_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class lookup_verifier_class =
{
  STATIC_HEADER,
  NULL,
  "lookup_verifier",
  sizeof(struct lookup_verifier),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

