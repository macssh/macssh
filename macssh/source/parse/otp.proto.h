void otpgenerator(char *, char *, char *, short);
void otphashlen(short, char *, char *);
void otphash(short, char *);
char *otpbtoh (char *, char *);
char *otpbtoe(char *, char *);
unsigned long otpextract(char *, short, short);
void otpinterface(short, char *, struct WindRec *, short, short, char *, short);
void otpgetselection(char *);

// SHA1 protos

typedef struct {
    unsigned long state[5];
    unsigned long count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void SHA1Transform(unsigned long state[5], unsigned char buffer[64]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned int len);
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

