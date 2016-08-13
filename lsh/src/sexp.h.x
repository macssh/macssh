/*
CLASS:sexp:
*/
#ifndef GABA_DEFINE
struct sexp
{
  struct lsh_object super;
  struct sexp_iterator * (*(iter))(struct sexp *self);
  struct lsh_string * (*(format))(struct sexp *self,int style,unsigned indent);
};
extern struct lsh_class sexp_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class sexp_class =
{
  STATIC_HEADER,
  NULL,
  "sexp",
  sizeof(struct sexp),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:sexp_iterator:
*/
#ifndef GABA_DEFINE
struct sexp_iterator
{
  struct lsh_object super;
  struct sexp * (*(get))(struct sexp_iterator *self);
  struct sexp * (*(assoc))(struct sexp_iterator *self,UINT32 length,const UINT8 *name,struct sexp_iterator **i);
  unsigned (*(left))(struct sexp_iterator *self);
  void (*(next))(struct sexp_iterator *self);
};
extern struct lsh_class sexp_iterator_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
struct lsh_class sexp_iterator_class =
{
  STATIC_HEADER,
  NULL,
  "sexp_iterator",
  sizeof(struct sexp_iterator),
  NULL,
  NULL,
};
#endif /* !GABA_DECLARE */

