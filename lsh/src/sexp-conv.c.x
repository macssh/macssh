static struct lsh_object *
make_sexp_conv(struct command *read,
  struct command *select,
  struct command *transform,
  struct command *print,
  struct abstract_write *dest)
  /* (B (print dest) (B* transform select read)) */
#define A GABA_APPLY
#define I GABA_VALUE_I
#define K GABA_VALUE_K
#define K1 GABA_APPLY_K_1
#define S GABA_VALUE_S
#define S1 GABA_APPLY_S_1
#define S2 GABA_APPLY_S_2
#define B GABA_VALUE_B
#define B1 GABA_APPLY_B_1
#define B2 GABA_APPLY_B_2
#define C GABA_VALUE_C
#define C1 GABA_APPLY_C_1
#define C2 GABA_APPLY_C_2
#define Sp GABA_VALUE_Sp
#define Sp1 GABA_APPLY_Sp_1
#define Sp2 GABA_APPLY_Sp_2
#define Sp3 GABA_APPLY_Sp_3
#define Bp GABA_VALUE_Bp
#define Bp1 GABA_APPLY_Bp_1
#define Bp2 GABA_APPLY_Bp_2
#define Bp3 GABA_APPLY_Bp_3
#define Cp GABA_VALUE_Cp
#define Cp1 GABA_APPLY_Cp_1
#define Cp2 GABA_APPLY_Cp_2
#define Cp3 GABA_APPLY_Cp_3
{
  return MAKE_TRACE("make_sexp_conv", 
    B2(A(((struct lsh_object *) print),
        ((struct lsh_object *) dest)),
      Bp3(((struct lsh_object *) transform),
        ((struct lsh_object *) select),
        ((struct lsh_object *) read))));
}
#undef A
#undef I
#undef K
#undef K1
#undef S
#undef S1
#undef S2
#undef B
#undef B1
#undef B2
#undef C
#undef C1
#undef C2
#undef Sp
#undef Sp1
#undef Sp2
#undef Sp3
#undef Bp
#undef Bp1
#undef Bp2
#undef Bp3
#undef Cp
#undef Cp1
#undef Cp2
#undef Cp3
/*
CLASS:sexp_select_command:command
*/
#ifndef GABA_DEFINE
struct sexp_select_command
{
  struct command super;
  struct int_list *ops;
};
extern struct lsh_class sexp_select_command_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_sexp_select_command_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct sexp_select_command *i = (struct sexp_select_command *) o;
  mark((struct lsh_object *) i->ops);
}
struct lsh_class sexp_select_command_class =
{
  STATIC_HEADER,
  &(command_class),
  "sexp_select_command",
  sizeof(struct sexp_select_command),
  do_sexp_select_command_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

/*
CLASS:sexp_conv_options:
*/
#ifndef GABA_DEFINE
struct sexp_conv_options
{
  struct lsh_object super;
  sexp_argp_state input;
  sexp_argp_state output;
  int once;
  int mode;
  struct alist *algorithms;
  int hash;
  struct command *select;
  struct command *transform;
  struct command *print;
};
extern struct lsh_class sexp_conv_options_class;
#endif /* !GABA_DEFINE */

#ifndef GABA_DECLARE
static void
do_sexp_conv_options_mark(struct lsh_object *o,
void (*mark)(struct lsh_object *o))
{
  struct sexp_conv_options *i = (struct sexp_conv_options *) o;
  mark((struct lsh_object *) i->algorithms);
  mark((struct lsh_object *) i->select);
  mark((struct lsh_object *) i->transform);
  mark((struct lsh_object *) i->print);
}
struct lsh_class sexp_conv_options_class =
{
  STATIC_HEADER,
  NULL,
  "sexp_conv_options",
  sizeof(struct sexp_conv_options),
  do_sexp_conv_options_mark,
  NULL,
};
#endif /* !GABA_DECLARE */

