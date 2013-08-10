#include "sudoku.h"
#include <stdio.h>
#include "io.h"

static void row(io_writer *w, const char nums[9]);

void sudoku_print(sudoku *pz, size_t (*write)(void *, size_t, size_t, void *), void *data)
{
	io_writer wv, *w;
	int i;

	wv.write = write;
	wv.data = data;

	w = &wv;

	for(i = 0; i < 9; i++) {
		if(i % 3 == 0 && i != 0)
			io_printf(w, "-------+-------+-------\n");
		row(w, pz->fields + i * 9);
	}
}

void row(io_writer *w, const char nums[9])
{
	io_printf(w, " %c %c %c | %c %c %c | %c %c %c\n",
		nums[0], nums[1], nums[2],
		nums[3], nums[4], nums[5],
		nums[6], nums[7], nums[8]);
}

size_t sudoku_fwrite(void *ptr, size_t size, size_t nitems, void *stream)
{
	return fwrite(ptr, size, nitems, (FILE *)stream);
}
