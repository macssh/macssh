/* dump-hex.c
 *
 * This program copies data from stdin to stdout, and also
 * writes a hex dump to stderr.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <errno.h>
#include <unistd.h>

static void
usage(void)
{
  fprintf(stderr,
	  "Usage: dump-hex\n"
	  "   or  dump-hex LABEL\n"
	  "   or  dump-hex --help\n\n"
	  "Copies data from stdin to stdout,\n"
	  "and also writes a hex dump to stderr.\n");
}

static int is_help_option(const char *arg)
{
  return !strcmp(arg, "-?") || !strcasecmp(arg, "--help");
}

/* Blocks for at least one character, but no longer. Returns number of
 * characters, just like read. */
static int
do_read(int fd, char *data, int length)
{
  int res;
  do
    res = read(fd, data, length);
  while ( (res < 0) && (errno == EINTR) );

  return res;
};

/* Writes all data, unless some error occurs. */
static int
do_write(int fd, const char *data, int length)
{
  int done;
  for (done = 0; done < length; )
  {
    int res = write(fd, data + done, length - done);
    if (res < 0)
    {
      if (errno != EINTR)
	return -1;
    }
    else
    {
      done += res;
    }
  }
  return done;
}

#define LINE 16

static int
output_line(int fd, const char *label,
	    unsigned pos, const char *data, unsigned length)
{
  /* The format used:
   *
   * LABEL   ADDRESS  DATA                                            ASCII
   * foobar: 000000B0 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff xxxxxxxxxxxxxxxx
   *
   *         0         1         2         3         4         5         6         7
   *         0123456789012345678901234567890123456789012345678901234567890123456789012
   *
   * Lines are 72 characters + the size of the label + newline.
   */

  const char hex_digit[16] = "0123456789abcdef";
  
  unsigned label_length = strlen(label);
  char *line = alloca(label_length + 80);
  unsigned i = 0;

  unsigned offset = pos % LINE;
  unsigned address = pos - offset;
  
  if (label_length)
    {
      /* Non-empty-label */
      memcpy(line, label, label_length);
      i = label_length;
      line[i++] = ':';
      line[i++] = ' ';
    }

  /* Write the address field. */
  {
    unsigned k;
    unsigned buffer;

    for (k = 8, buffer = address; k-- > 0; buffer >>= 4)
      line[i+k] = hex_digit[buffer & 0xf];

    i += 8;
  }
  line[i++] = ' ';

  /* Skip to the start of the data */
  memset(line + i, ' ', 3 * offset);
  i += 3 * offset;

  /* Write the hex data */
  {
    unsigned k;
    for (k = 0; k<length; k++)
      {
	unsigned char c = data[k];
	line[i++] = hex_digit[c >> 4];
	line[i++] = hex_digit[c & 0xf];
	line[i++] = ' ';
      }
  }

  /* Skip to ascii part */
  {
    /* (LINE - (length + offset)) * 3 + offset
     * = (LINE - length) * 3 - offset * 2
     */
    unsigned skip = 3* (LINE - length) - 2 * offset;
    memset(line + i, ' ', skip);
    i += skip;
  }

  /* Output ascii */
  {
    unsigned k;
    for (k = 0; k<length; k++)
      {
	unsigned char c = data[k];
	line[i++] = isprint(c) ? c : '.';
      }
  }

  /* Final newline */
  line[i++] = '\n';

  return do_write(fd, line, i);
}

int main(int argc, char **argv)
{
  const char *label;
  unsigned pos;
  
  switch (argc)
  {
  case 1:
    label = "";
    break;
  case 2:
    if (is_help_option(argv[1]))
    {
      usage();
      return EXIT_SUCCESS;
    }
    else label = argv[1];
    break;
  default:
    usage();
    return EXIT_FAILURE;
  }

  for (pos = 0;;)
  {
    unsigned char buf[LINE];

    unsigned left = LINE - (pos % LINE);
    int res;
    
    /* Try reading the rest of the current line. */
    res = do_read(STDIN_FILENO, buf, left);
    if (!res)
      /* EOF */
      return EXIT_SUCCESS;

    else if (res < 0)
      {
	fprintf(stderr, "%s: read error: %s\n", label, strerror(errno));
	return EXIT_FAILURE;
      }

    /* Dump it */
    if (output_line(STDERR_FILENO, label, pos, buf, res) <= 0)
      {
	fprintf(stderr, "%s: write error on stderr: %s\n", label, strerror(errno));
	return EXIT_FAILURE;
      }
    
    /* And pass it on */
    if (do_write(STDOUT_FILENO, buf, res) < 0)
      {
	fprintf(stderr, "%s: write error: %s\n", label, strerror(errno));
	return EXIT_FAILURE;
      }

    pos += res;
  }
}
