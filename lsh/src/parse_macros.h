/* parse_macros.h
 *
 * Utility macros for managing a struct simple_buffer *buffer.
 *
 * $Id$ */

#ifndef LSH_PARSE_MACROS_H_INCLUDED
#define LSH_PARSE_MACROS_H_INCLUDED

#ifndef BUFFER
#define BUFFER buffer
#endif

#define LEFT (BUFFER->capacity - BUFFER->pos)
#define HERE (BUFFER->data + BUFFER->pos)
#define ADVANCE(n) (BUFFER->pos += (n))
#define GET() (BUFFER->data[BUFFER->pos++])

#endif /* LSH_PARSE_MACROS_H_INCLUDED */
