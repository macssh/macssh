/* werror.c
 *
 * Simple diagnostics routines.
 *
 * $id$
 */

#include "werror.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
werror(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  
  fprintf(stderr, "%s: ", werror_program_name);
  vfprintf(stderr, format, args);

  va_end(args);
}

void
fatal(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  
  fprintf(stderr, "%s: ", werror_program_name);
  vfprintf(stderr, format, args);

  va_end(args);

  exit(EXIT_FAILURE);
}
