/*
CLASS:srp_entry:
*/
#ifndef GABA_DEFINE
struct srp_entry
{
  struct lsh_object super;
  struct lsh_string *name;
  struct lsh_string *salt;
  mpz_t verifier;
};
extern struct lsh_class srp_entry_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_srp_entry_free(struct lsh_object *o)
{
  struct srp_entry *i = (struct srp_entry *) o;
  lsh_string_free(i->name);
  lsh_string_free(i->salt);
  mpz_clear(i->verifier);
}
struct lsh_class srp_entry_class =
{
  STATIC_HEADER,
  NULL,
  "srp_entry",
  sizeof(struct srp_entry),
  NULL,
  do_srp_entry_free,
};
#endif /* !GABA_DECLARE */

