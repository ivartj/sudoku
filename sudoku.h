#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>

typedef struct sudoku sudoku;

struct sudoku {
	unsigned char fields[81];
};

sudoku *sudoku_parse(size_t (*read)(void *, size_t, size_t, void *), void *data);

void sudoku_solve(sudoku *pz);

size_t sudoku_fread(void *ptr, size_t size, size_t nitems, void *stream);
size_t sudoku_fwrite(void *ptr, size_t size, size_t nitems, void *stream);

#endif
