/* werror.h
 *
 * Simple diagnostics routines.
 *
 * $id$
 */

#ifndef SFTP_WERROR_H_INCLUDED
#define SFTP_WERROR_H_INCLUDED

/* For types and config stuff */
#include "buffer.h"

extern const char *werror_program_name;

void werror(const char *format, ...) PRINTF_STYLE(1,2);
void fatal(const char *format, ...) PRINTF_STYLE(1,2) NORETURN;

#endif /* SFTP_WERROR_H_INCLUDED */

