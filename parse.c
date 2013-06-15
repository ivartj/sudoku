#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

sudoku *sudoku_parse(size_t (*read)(void *, size_t, size_t, void *), void *data)
{
	io_reader rv, *r;
	int c;
	int num;
	sudoku *pz;

	rv.read = read;
	rv.data = data;

	r = &rv;

	num = 0;
	pz = calloc(1, sizeof(sudoku));
	while((c = io_getc(r)) != EOF && num < 81) {
		switch(c) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '_':
			pz->fields[num++] = c;
			break;
		}
	}

	if(num != 81) {
		free(pz);
		return NULL;
	}

	return pz;
}
