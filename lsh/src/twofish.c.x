                            2  �   	  �    #ifndef GABA_DEFINEstruct twofish_instance{  struct crypto_instance super;  TWOFISH_context ctx;};extern struct lsh_class twofish_instance_class;#endif /* !GABA_DEFINE */#ifndef GABA_DECLAREstruct lsh_class twofish_instance_class ={ STATIC_HEADER,  &crypto_instance_class, "twofish_instance", sizeof(struct twofish_instance),  NULL,  NULL};#endif /* !GABA_DECLARE */TEXTCWIE � A                  