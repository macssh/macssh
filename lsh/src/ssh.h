/* ssh.h
 *
 */

#ifndef LSH_SSH_H_INCLUDED
#define LSH_SSH_H_INCLUDED

/* 1-19 Transport layer generic (e.g. disconnect, ignore, debug, etc) */

#define SSH_MSG_DISCONNECT             1
#define SSH_MSG_IGNORE                 2
#define SSH_MSG_UNIMPLEMENTED          3
#define SSH_MSG_DEBUG                  4
#define SSH_MSG_SERVICE_REQUEST        5
#define SSH_MSG_SERVICE_ACCEPT         6

/* 20-29 Algorithm negotiation */

#define SSH_MSG_KEXINIT                20
#define SSH_MSG_NEWKEYS                21

/* 30-49 Key exchange method specific (numbers can be reused for
 *       different authentication methods) */

#define SSH_MSG_KEXDH_INIT             30
#define SSH_MSG_KEXDH_REPLY            31

/* SRP based key exchange (experimental) */

#define SSH_MSG_KEXSRP_INIT            30
#define SSH_MSG_KEXSRP_REPLY           31
#define SSH_MSG_KEXSRP_PROOF           32

/* Diffie-hellman with group negotiation (experimental) */

#define SSH_MSG_KEX_DH_MOD_GROUPS      30
#define SSH_MSG_KEX_DH_MOD_INIT        31
#define SSH_MSG_KEX_DH_MOD_REPLY       32

/* 50-59 User authentication generic */

#define SSH_FIRST_USERAUTH_GENERIC 50

#define SSH_MSG_USERAUTH_REQUEST 50 
#define SSH_MSG_USERAUTH_FAILURE 51 
#define SSH_MSG_USERAUTH_SUCCESS 52 
#define SSH_MSG_USERAUTH_BANNER 53

/* 60-79 User authentication method specific (numbers can be reused
 * for different authentication methods) */

#define SSH_FIRST_USERAUTH_SPECIFIC 60

#define SSH_MSG_USERAUTH_PK_OK 60
#define SSH_MSG_USERAUTH_PASSWD_CHANGEREQ 60

/* 80-89 Connection protocol generic */

#define SSH_FIRST_CONNECTION_GENERIC 80

#define SSH_MSG_GLOBAL_REQUEST 80 
#define SSH_MSG_REQUEST_SUCCESS 81 
#define SSH_MSG_REQUEST_FAILURE 82 

/* 90-127 Channel related messages */

#define SSH_FIRST_CONNECTION_CHANNELS 90

#define SSH_MSG_CHANNEL_OPEN 90 
#define SSH_MSG_CHANNEL_OPEN_CONFIRMATION 91 
#define SSH_MSG_CHANNEL_OPEN_FAILURE 92 
#define SSH_MSG_CHANNEL_WINDOW_ADJUST 93 
#define SSH_MSG_CHANNEL_DATA 94 
#define SSH_MSG_CHANNEL_EXTENDED_DATA 95 
#define SSH_MSG_CHANNEL_EOF 96 
#define SSH_MSG_CHANNEL_CLOSE 97 
#define SSH_MSG_CHANNEL_REQUEST 98 
#define SSH_MSG_CHANNEL_SUCCESS 99 
#define SSH_MSG_CHANNEL_FAILURE 100

/* 128-191 Reserved */

#define SSH_FIRST_RESERVED 128

/* 192-255 Local extensions */
#define SSH_FIRST_LOCAL 192

/* Disconnecting */

#define SSH_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT      1
#define SSH_DISCONNECT_PROTOCOL_ERROR                   2
#define SSH_DISCONNECT_KEY_EXCHANGE_FAILED              3

/* Formely known as SSH_DISCONNECT_HOST_AUTHENTICATION_FAILED */
#define SSH_DISCONNECT_RESERVED                         4

#define SSH_DISCONNECT_MAC_ERROR                        5
#define SSH_DISCONNECT_COMPRESSION_ERROR                6
#define SSH_DISCONNECT_SERVICE_NOT_AVAILABLE            7
#define SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED   8
#define SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE          9
#define SSH_DISCONNECT_CONNECTION_LOST                 10
#define SSH_DISCONNECT_BY_APPLICATION                  11
#define SSH_DISCONNECT_TOO_MANY_CONNECTIONS            12
#define SSH_DISCONNECT_AUTH_CANCELLED_BY_USER          13
#define SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE  14
#define SSH_DISCONNECT_ILLEGAL_USER_NAME               15

/* Channels */
#define SSH_OPEN_ADMINISTRATIVELY_PROHIBITED 1 
#define SSH_OPEN_CONNECT_FAILED 2 
#define SSH_OPEN_UNKNOWN_CHANNEL_TYPE 3 
#define SSH_OPEN_RESOURCE_SHORTAGE 4

/* Extended data */

#define SSH_EXTENDED_DATA_STDERR 1

/* pty op.codes */
#define SSH_TTY_OP_END     0
#define SSH_VINTR          1
#define SSH_VQUIT          2
#define SSH_VERASE         3
#define SSH_VKILL          4
#define SSH_VEOF           5
#define SSH_VEOL           6
#define SSH_VEOL2          7
#define SSH_VSTART         8
#define SSH_VSTOP          9
#define SSH_VSUSP          10
#define SSH_VDSUSP         11
#define SSH_VREPRINT       12
#define SSH_VWERASE        13
#define SSH_VLNEXT         14
#define SSH_VFLUSH         15
#define SSH_VSWTCH         16
#define SSH_VSTATUS        17
#define SSH_VDISCARD       18
#define SSH_IGNPAR         30
#define SSH_PARMRK         31
#define SSH_INPCK          32
#define SSH_ISTRIP         33
#define SSH_INLCR          34
#define SSH_IGNCR          35
#define SSH_ICRNL          36
#define SSH_IUCLC          37
#define SSH_IXON           38
#define SSH_IXANY          39
#define SSH_IXOFF          40
#define SSH_IMAXBEL        41
#define SSH_ISIG           50
#define SSH_ICANON         51
#define SSH_XCASE          52
#define SSH_ECHO           53
#define SSH_ECHOE          54
#define SSH_ECHOK          55
#define SSH_ECHONL         56
#define SSH_NOFLSH         57
#define SSH_TOSTOP         58
#define SSH_IEXTEN         59
#define SSH_ECHOCTL        60
#define SSH_ECHOKE         61
#define SSH_PENDIN         62
#define SSH_OPOST          70
#define SSH_OLCUC          71
#define SSH_ONLCR          72
#define SSH_OCRNL          73
#define SSH_ONOCR          74
#define SSH_ONLRET         75
#define SSH_CS7            90
#define SSH_CS8            91
#define SSH_PARENB         92
#define SSH_PARODD         93
#define SSH_TTY_OP_ISPEED  128
#define SSH_TTY_OP_OSPEED  129
#define SSH_TTY_OP_RESERVED 160

/* Limits */

/* Default max length of packet payload */
#define SSH_MAX_PACKET 0x8000

/* Add this to get the maximum total length, including length,
 * padding, MAC etc */
#define SSH_MAX_PACKET_FUZZ (35000 - SSH_MAX_PACKET)

/* Subtract this to get a reasonable max_packet value for a channel. */
#define SSH_CHANNEL_MAX_PACKET_FUZZ 100

#endif /* LSH_SSH_H_INCLUDED */
