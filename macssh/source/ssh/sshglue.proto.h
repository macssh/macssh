void ssh_glue_initial(WindRec*);
short ssh_glue_installed(void);
void ssh_glue_write(struct WindRec*, unsigned char*, short);
void ssh_glue_read(struct WindRec*, unsigned char*, short);
void ssh_glue_close(WindRec*);
void ssh_glue_randomize(void);
void ssh_glue_exportkey(void);
void ssh_glue_wresize(struct WindRec*);
