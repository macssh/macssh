/* sftp.h
 */

#ifndef SFTP_SFTP_H_INCLUDED
#define SFTP_SFTP_H_INCLUDED

/* Messages */
enum sftp_msg_type {
  SSH_FXP_INIT           =   1, /* 0x01 */
  SSH_FXP_VERSION        =   2, /* 0x02 */
  SSH_FXP_OPEN           =   3, /* 0x03 */
  SSH_FXP_CLOSE          =   4, /* 0x04 */
  SSH_FXP_READ           =   5, /* 0x05 */
  SSH_FXP_WRITE          =   6, /* 0x06 */
  SSH_FXP_LSTAT          =   7, /* 0x07 */
  SSH_FXP_FSTAT          =   8, /* 0x08 */
  SSH_FXP_SETSTAT        =   9, /* 0x09 */
  SSH_FXP_FSETSTAT       =  10, /* 0x0a */
  SSH_FXP_OPENDIR        =  11, /* 0x0b */
  SSH_FXP_READDIR        =  12, /* 0x0c */
  SSH_FXP_REMOVE         =  13, /* 0x0d */
  SSH_FXP_MKDIR          =  14, /* 0x0e */
  SSH_FXP_RMDIR          =  15, /* 0x0f */
  SSH_FXP_REALPATH       =  16, /* 0x10 */
  SSH_FXP_STAT           =  17, /* 0x11 */
  SSH_FXP_RENAME         =  18, /* 0x12 */
  SSH_FXP_STATUS         = 101, /* 0x65 */
  SSH_FXP_HANDLE         = 102, /* 0x66 */
  SSH_FXP_DATA           = 103, /* 0x67 */
  SSH_FXP_NAME           = 104, /* 0x68 */
  SSH_FXP_ATTRS          = 105, /* 0x69 */
  SSH_FXP_EXTENDED       = 200, /* 0xc8 */
  SSH_FXP_EXTENDED_REPLY = 201, /* 0xc9 */
};

/* Attribute flags */
enum sftp_attr_flag {
  SSH_FILEXFER_ATTR_SIZE        = 0x00000001,
  SSH_FILEXFER_ATTR_UIDGID      = 0x00000002,
  SSH_FILEXFER_ATTR_PERMISSIONS = 0x00000004,
  SSH_FILEXFER_ATTR_ACMODTIME   = 0x00000008,
  SSH_FILEXFER_ATTR_EXTENDED    = 0x80000000,
};

/* Open flags */
enum sftp_open_flag {
  SSH_FXF_READ   = 0x00000001,
  SSH_FXF_WRITE  = 0x00000002,
  SSH_FXF_APPEND = 0x00000004,
  SSH_FXF_CREAT  = 0x00000008,
  SSH_FXF_TRUNC  = 0x00000010,
  SSH_FXF_EXCL   = 0x00000020,
};

/* Status codes */
enum sftp_status {
  SSH_FX_OK                = 0,
  SSH_FX_EOF               = 1,
  SSH_FX_NO_SUCH_FILE      = 2,
  SSH_FX_PERMISSION_DENIED = 3,
  SSH_FX_FAILURE           = 4,
  SSH_FX_BAD_MESSAGE       = 5,
#if 0
  SSH_FX_NO_CONNECTION     = 6,
  SSH_FX_CONNECTION_LOST   = 7,
#endif
  SSH_FX_OP_UNSUPPORTED    = 8,
};

#endif /* SFTP_SFTP_H_INCLUDED */
