#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdarg.h>

typedef struct io_buf io_buf;
typedef struct io_writer io_writer;
typedef struct io_reader io_reader;

struct io_buf {
	size_t len;
	size_t cap;
	size_t off;
	void *buf;
};

struct io_writer {
	void *data;
	size_t (*write)(void *ptr, size_t size, size_t nitems, void *data);
};

struct io_reader {
	void *data;
	size_t (*read)(void *ptr, size_t size, size_t nitems, void *data);
};

int io_vprintf(io_writer *io, const char *format, va_list ap);
int io_printf(io_writer *io, const char *format, ...);

/* unlike fputc, returns number of bytes written */
int io_putc(io_writer *io, unsigned char c);
int io_getc(io_reader *r);

void *io_readall(int fd, unsigned *rlen);

size_t io_bufwrite(void *data, size_t size, size_t nmemb, io_buf *buf);
size_t io_bufread(void *data, size_t size, size_t nmemb, io_buf *buf);
int io_vasprintf(char **ptr, const char *format, va_list ap);

#endif
