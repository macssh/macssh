void otpmd4init (struct otpmdx_ctx *context);
void otpmd4update (struct otpmdx_ctx *context, unsigned char *input, unsigned short inputLen);
void otpmd4final(unsigned char *digest, struct otpmdx_ctx *context);
