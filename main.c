#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sudoku.h"

static void usage(FILE *out);
static void parseargs(int argc, char *argv[]);
static void openfile(void);
static void parse(void);
static void solve(void);
static void print(void);

sudoku *puzzle;
char *filename;
FILE *file;

void usage(FILE *out)
{
	fprintf(out, "Usage: sudokusolve <puzzle file>\n");
}

void parseargs(int argc, char *argv[])
{
	int c;
	static struct option longopts[] = {
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	while((c = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
		switch(c) {
		case 'h':
			usage(stdout);
			exit(EXIT_SUCCESS);
		case '?':
			usage(stderr);
			exit(EXIT_FAILURE);
		}
	}

	if(optind != argc - 1) {
		usage(stderr);
		exit(EXIT_FAILURE);
	}

	filename = argv[optind];
}

void openfile(void)
{
	file = fopen(filename, "r");
	if(file == NULL) {
		fprintf(stderr, "Failed to open '%s': %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void closefile(void)
{
	fclose(file);
}

void parse(void)
{
	puzzle = sudoku_parse(fread, file);
	if(puzzle == NULL) {
		fprintf(stderr, "Failed to parse file.\n");
		exit(EXIT_FAILURE);
	}
}

void solve(void)
{
	sudoku_solve(puzzle);
}

void print(void)
{
	sudoku_print(puzzle, fwrite, stdout);
}

int main(int argc, char *argv[])
{
	parseargs(argc, argv);
	openfile();
	atexit(closefile);
	parse();
	solve();
	print();
	exit(EXIT_SUCCESS);
}
