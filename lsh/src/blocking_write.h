/* blocking_write.h
 *
 */

#ifndef LSH_BLOCKING_WRITE_H_INCLUDED
#define LSH_BLOCKING_WRITE_H_INCLUDED

#include "abstract_io.h"

struct abstract_write *
make_blocking_write(int fd, int with_nonblocking,
		    struct exception_handler *e);

#endif /* LSH_BLOCKING_WRITE_H_INCLUDED */
