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
char *filename = NULL;
FILE *file = NULL;
char *outfile = NULL;
FILE *out = NULL;

void usage(FILE *out)
{
	fprintf(out, "Usage: sudokusolve [ -o <output-file> ] [ <puzzle file> ]\n");
}

void parseargs(int argc, char *argv[])
{
	int c;
	static struct option longopts[] = {
		{ "help", no_argument, 0, 'h' },
		{ "out", required_argument, 0, 'o' },
		{ 0, 0, 0, 0 }
	};

	while((c = getopt_long(argc, argv, "ho:", longopts, NULL)) != -1)
	switch(c) {
	case 'o':
		outfile = optarg;
		break;
	case 'h':
		usage(stdout);
		exit(EXIT_SUCCESS);
	case '?':
		usage(stderr);
		exit(EXIT_FAILURE);
	}

	
	switch(argc - optind) {
	case 0:
		break;
	case 1:
		filename = argv[optind];
		break;
	default:
		usage(stderr);
		exit(EXIT_FAILURE);
	}
}

void openfile(void)
{
	if(filename != NULL) {
		file = fopen(filename, "r");
		if(file == NULL) {
			fprintf(stderr, "Failed to open '%s': %s\n", filename, strerror(errno));
			exit(EXIT_FAILURE);
		}
	} else
		file = stdin;

	if(outfile != NULL) {
		out = fopen(outfile, "w");
		if(out == NULL) {
			fprintf(stderr, "Failed to open '%s' for writing: %s\n", outfile, strerror(errno));
			exit(EXIT_FAILURE);
		}
	} else
		out = stdout;
}

void closefile(void)
{
	fclose(file);
}

void parse(void)
{
	puzzle = sudoku_parse(sudoku_fread, file);
	if(puzzle == NULL) {
		fprintf(stderr, "Failed to parse input.\n");
		exit(EXIT_FAILURE);
	}
}

void solve(void)
{
	sudoku_solve(puzzle);
}

void print(void)
{
	sudoku_print(puzzle, sudoku_fwrite, out);
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
