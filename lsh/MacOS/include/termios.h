#ifndef __TERMIOS__
#define __TERMIOS__

#ifndef __const
#define __const const
#endif

typedef unsigned char   cc_t;
typedef unsigned int    speed_t;
typedef unsigned int    tcflag_t;

/* Return the output baud rate stored in *TERMIOS_P.  */
extern speed_t cfgetospeed __P ((__const struct termios *__termios_p));

/* Return the input baud rate stored in *TERMIOS_P.  */
extern speed_t cfgetispeed __P ((__const struct termios *__termios_p));

/* Set the output baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetospeed __P ((struct termios *__termios_p, speed_t __speed));

/* Set the input baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetispeed __P ((struct termios *__termios_p, speed_t __speed));

/* Put the state of FD into *TERMIOS_P.  */
extern int tcgetattr __P ((int __fd, struct termios *__termios_p));

/* Set the state of FD to *TERMIOS_P.
   Values for OPTIONAL_ACTIONS (TCSA*) are in <bits/termios.h>.  */
extern int tcsetattr __P ((int __fd, int __optional_actions,
                           __const struct termios *__termios_p));

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif
/*
 * termios type and macro definitions.  Be careful about adding stuff
 * to this file since it's used in GNU libc and there are strict rules
 * concerning namespace pollution.
 */

#define NCCS 32
struct termios {
        tcflag_t c_iflag;               /* input mode flags */
        tcflag_t c_oflag;               /* output mode flags */
        tcflag_t c_cflag;               /* control mode flags */
        tcflag_t c_lflag;               /* local mode flags */
        cc_t c_line;                    /* line discipline (== c_cc[19]) */
        cc_t c_cc[NCCS];                /* control characters */
        speed_t c_ispeed;               /* input speed */
        speed_t c_ospeed;               /* output speed */
};
#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

/* c_cc characters */
#define VINTR   0
#define VQUIT   1
#define VERASE  2
#define VKILL   3
#define VEOF    4
#define VMIN    5
#define VEOL    6
#define VTIME   7
#define VEOL2   8
#define VSWTC   9

#define VWERASE         10
#define VREPRINT        11
#define VSUSP           12
#define VSTART          13
#define VSTOP           14
#define VLNEXT          15
#define VDISCARD        16

/* c_iflag bits */
#define IGNBRK  0000001
#define BRKINT  0000002
#define IGNPAR  0000004
#define PARMRK  0000010
#define INPCK   0000020
#define ISTRIP  0000040
#define INLCR   0000100
#define IGNCR   0000200
#define ICRNL   0000400
#define IXON    0001000
#define IXOFF   0002000
/* POSIX.1 doesn't want these... */
#ifdef __USE_BSD
# define IXANY          0004000
# define IUCLC          0010000
# define IMAXBEL        0020000
#endif

/* c_oflag bits */
#define OPOST   0000001
#define ONLCR   0000002
#define OLCUC   0000004

#define OCRNL   0000010
#define ONOCR   0000020
#define ONLRET  0000040

#define OFILL   00000100
#define OFDEL   00000200
#define NLDLY   00001400
#define   NL0   00000000
#define   NL1   00000400
#define   NL2   00001000
#define   NL3   00001400
#define TABDLY  00006000
#define   TAB0  00000000
#define   TAB1  00002000
#define   TAB2  00004000
#define   TAB3  00006000
#define CRDLY   00030000
#define   CR0   00000000
#define   CR1   00010000
#define   CR2   00020000
#define   CR3   00030000
#define FFDLY   00040000
#define   FF0   00000000
#define   FF1   00040000
#define BSDLY   00100000
#define   BS0   00000000
#define   BS1   00100000
#define VTDLY   00200000
#define   VT0   00000000
#define   VT1   00200000
#define XTABS   01000000 /* Hmm.. Linux/i386 considers this part of TABDLY.. */

/* c_cflag bit meaning */
#define CBAUD   0000377
#define  B0     0000000         /* hang up */
#define  B50    0000001
#define  B75    0000002
#define  B110   0000003
#define  B134   0000004
#define  B150   0000005
#define  B200   0000006
#define  B300   0000007
#define  B600   0000010
#define  B1200  0000011
#define  B1800  0000012
#define  B2400  0000013
#define  B4800  0000014
#define  B9600  0000015
#define  B19200 0000016
#define  B38400 0000017
#define EXTA B19200
#define EXTB B38400
#define CBAUDEX 0000020
#define  B57600   00020
#define  B115200  00021
#define  B230400  00022
#define  B460800  00023
#define  B500000  00024
#define  B576000  00025
#define  B921600  00026
#define  B1000000 00027
#define  B1152000 00030
#define  B1500000 00031
#define  B2000000 00032
#define  B2500000 00033
#define  B3000000 00034
#define  B3500000 00035
#define  B4000000 00036

#define CSIZE   00001400
#define   CS5   00000000
#define   CS6   00000400
#define   CS7   00001000
#define   CS8   00001400

#define CSTOPB  00002000
#define CREAD   00004000
#define PARENB  00010000
#define PARODD  00020000
#define HUPCL   00040000

#define CLOCAL  00100000
#define CRTSCTS   020000000000          /* flow control */

/* c_lflag bits */
#define ISIG    0x00000080
#define ICANON  0x00000100
#define XCASE   0x00004000
#define ECHO    0x00000008
#define ECHOE   0x00000002
#define ECHOK   0x00000004
#define ECHONL  0x00000010
#define NOFLSH  0x80000000
#define TOSTOP  0x00400000
#define ECHOCTL 0x00000040
#define ECHOPRT 0x00000020
#define ECHOKE  0x00000001
#define FLUSHO  0x00800000
#define PENDIN  0x20000000
#define IEXTEN  0x00000400

/* Values for the ACTION argument to `tcflow'.  */
#define TCOOFF          0
#define TCOON           1
#define TCIOFF          2
#define TCION           3

/* Values for the QUEUE_SELECTOR argument to `tcflush'.  */
#define TCIFLUSH        0
#define TCOFLUSH        1
#define TCIOFLUSH       2

/* Values for the OPTIONAL_ACTIONS argument to `tcsetattr'.  */
#define TCSANOW         0
#define TCSADRAIN       1
#define TCSAFLUSH       2

struct sgttyb {
        char    sg_ispeed;
        char    sg_ospeed;
        char    sg_erase;
        char    sg_kill;
        short   sg_flags;
};

struct tchars {
        char    t_intrc;
        char    t_quitc;
        char    t_startc;
        char    t_stopc;
        char    t_eofc;
        char    t_brkc;
};

struct ltchars {
        char    t_suspc;
        char    t_dsuspc;
        char    t_rprntc;
        char    t_flushc;
        char    t_werasc;
        char    t_lnextc;
};

/* Used for packet mode */
/* already defined in ttycom.h
#define TIOCPKT_DATA             0
#define TIOCPKT_FLUSHREAD        1
#define TIOCPKT_FLUSHWRITE       2
#define TIOCPKT_STOP             4
#define TIOCPKT_START            8
#define TIOCPKT_NOSTOP          16
#define TIOCPKT_DOSTOP          32
*/

/* already defined in ttycom.h
struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
};
*/

#define NCC 10
struct termio {
        unsigned short c_iflag;         /* input mode flags */
        unsigned short c_oflag;         /* output mode flags */
        unsigned short c_cflag;         /* control mode flags */
        unsigned short c_lflag;         /* local mode flags */
        unsigned char c_line;           /* line discipline */
        unsigned char c_cc[NCC];        /* control characters */
};

/* c_cc characters */
#define _VINTR  0
#define _VQUIT  1
#define _VERASE 2
#define _VKILL  3
#define _VEOF   4
#define _VMIN   5
#define _VEOL   6
#define _VTIME  7
#define _VEOL2  8
#define _VSWTC  9

/* modem lines */
/* already defined in ttycom.h
#define TIOCM_LE        0x001
#define TIOCM_DTR       0x002
#define TIOCM_RTS       0x004
#define TIOCM_ST        0x008
#define TIOCM_SR        0x010
#define TIOCM_CTS       0x020
#define TIOCM_CAR       0x040
#define TIOCM_RNG       0x080
#define TIOCM_DSR       0x100
#define TIOCM_CD        TIOCM_CAR
#define TIOCM_RI        TIOCM_RNG
*/

/* ioctl (fd, TIOCSERGETLSR, &result) where result may be as below */
#define TIOCSER_TEMT    0x01    /* Transmitter physically empty */

/* line disciplines */
#define N_TTY           0
#define N_SLIP          1
#define N_MOUSE         2
#define N_PPP           3
#define N_STRIP         4
#define N_AX25          5
#define N_X25           6       /* X.25 async  */
#define N_6PACK         7
#define N_MASC          8       /* Mobitex module  */
#define N_R3964         9       /* Simatic R3964 module  */
#define N_PROFIBUS_FDL  10      /* Profibus  */
#define N_IRDA          11      /* Linux IR  */
#define N_SMSBLOCK      12      /* SMS block mode  */
#define N_HDLC          13      /* synchronous HDLC  */
#define N_SYNC_PPP      14      /* synchronous PPP  */

#endif /* __TERMIOS__ */
