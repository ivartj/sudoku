#include "sudoku.h"
#include <stdint.h>
#include <assert.h>

#define MASK 0x1FF

void solve(uint16_t m[81]);
void exverti(uint16_t m[81], int i);
void exhoriz(uint16_t m[81], int i);
void exregio(uint16_t m[81], int i);
void elverti(uint16_t m[81], int i);
void elhoriz(uint16_t m[81], int i);
void elregio(uint16_t m[81], int rg);
void exregiox(uint16_t m[81], int rg);
void elvertix(uint16_t m[81], int n, int c, int rg);
void elhorizx(uint16_t m[81], int n, int r, int rg);
void elregiov(uint16_t m[81], int n, int rg, int xc);

typedef struct ser ser;

struct ser {
	uint16_t m[9];
};

int solved(uint16_t f)
{
	int i, num, ms;

	ms = 0;
	for(i = 0; i < 9; i++)
		if((f >> i) & 1) {
			num = i + 1;
			ms++;
		}
	if(ms == 1) {
		return num;
	}
	return 0;
}

void sudoku_solve(sudoku *s)
{
	int i, num;
	uint16_t m[81];

	for(i = 0; i < 81; i++) {
		if(s->fields[i] == '_') {
			m[i] = MASK;
			continue;
		}
		m[i] = 1 << (s->fields[i] - '0' - 1);
	}

	solve(m);

	for(i = 0; i < 81; i++) {
		if((num = solved(m[i])) != 0)
			s->fields[i] = '0' + num;
		else
			s->fields[i] = '_';
	}
}

void solve(uint16_t m[81])
{
	int i, j;

	for(j = 0; j < 100; j++) {
		for(i = 0; i < 81; i++) {
			if(!solved(m[i]))
				continue;
			//exhoriz(m, i);
			//exverti(m, i);
			exregio(m, i);
		}

		for(i = 0; i < 9; i++) {
			elhoriz(m, i);
			elverti(m, i);
			elregio(m, i);
			exregiox(m, i);
		}
	}
}

uint16_t ex(uint16_t i, uint16_t f)
{
	return f & ~i;
}

void exhoriz(uint16_t m[81], int i)
{
	int o, c, f;

	o = i / 9;
	for(c = 0; c < 9; c++) {
		f = c + o * 9;
		if(f == i)
			continue;
		m[f] = ex(m[i], m[f]);
	}
}

void exverti(uint16_t m[81], int i)
{
	int o, r, f;

	o = i % 9;
	for(r = 0; r < 9; r++) {
		f = r * 9 + o;
		if(f == i)
			continue;
		m[f] = ex(m[i], m[f]);
	}
}

void exregio(uint16_t m[81], int i)
{
	int r, c;
	int rr, cc;
	int f;

	c = (i % 9) / 3;
	r = (i / 9) / 3;

	for(rr = 0; rr < 3; rr++)
	for(cc = 0; cc < 3; cc++) {
		f = r * 27 + rr * 9 + c * 3 + cc;
		if(f == i)
			continue;
		m[f] = ex(m[i], m[f]);
	}
}

void elhoriz(uint16_t m[81], int r)
{
	int i, n, ms, c, l;

	for(n = 0; n < 9; n++) {
		ms = 0;
		for(c = 0; c < 9; c++) {
			i = r * 9 + c;
			if(m[i] & (1 << n)) {
				ms++;
				l = i;
			}
		}
		if(ms == 1)
			m[l] = 1 << n;
	}
}

void elverti(uint16_t m[81], int c)
{
	int i, n, ms, r, l;

	for(n = 0; n < 9; n++) {
		ms = 0;
		for(r = 0; r < 9; r++) {
			i = r * 9 + c;
			if(m[i] & (1 << n)) {
				ms++;
				l = i;
			}
		}
		if(ms == 1)
			m[l] = 1 << n;
	}
}

void elregio(uint16_t m[81], int rg)
{
	int c, r, i, rr, cc, n, ms, l;

	c = rg % 3;
	r = rg / 3;

	for(n = 0; n < 9; n++) {
		ms = 0;
		for(rr = 0; rr < 3; rr++)
		for(cc = 0; cc < 3; cc++) {
			i = r * 27 + rr * 9 + c * 3 + cc;
			if(m[i] & (1 << n)) {
				ms++;
				l = i;
			}
		}
		if(ms == 1)
			m[l] = 1 << n;
	}
}

ser reg(uint16_t m[81], int rg)
{
	ser s;
	int c, r, cc, rr;
	int i;

	c = rg % 3;
	r = rg / 3;
	i = 0;

	for(rr = 0; rr < 3; rr++)
	for(cc = 0; cc < 3; cc++)
		s.m[i++] = m[r * 27 + rr * 9 + c * 3 + cc];

	return s;
}

ser col(uint16_t m[81], int c)
{
	ser s;
	int r;

	for(r = 0; r < 9; r++)
		s.m[r] = m[r * 9 + c];

	return s;
}

ser row(uint16_t m[81], int r)
{
	ser s;
	int c;

	for(c = 0; c < 9; r++)
		s.m[c] = m[r * 9 + c];

	return s;
}

void exregiox(uint16_t m[81], int rg)
{
	int n;
	ser s;
	int colmask, rowmask;
	int i, c, r;

	s = reg(m, rg);

	for(n = 0; n < 9; n++) {
		colmask = 0;
		rowmask = 0;
		for(c = 0; c < 3; c++)
		for(r = 0; r < 3; r++) {
			i = r * 3 + c;
			if(s.m[i] & (1 << n)) {
				colmask |= 1 << c;
				rowmask |= 1 << r;
			}
		}


		if((c = solved(colmask)) != 0) {
			elvertix(m, n, (rg % 3) * 3 + c - 1, rg);
		}

		if((r = solved(rowmask)) != 0) {
			elhorizx(m, n, (rg / 3) * 3 + r - 1, rg);
		}
	}
}

void exvertix(uint16_t m[81], int c)
{
	int n;
	ser s;
	int regmask;
	int rg, r, i;

	s = col(m, c);

	for(n = 0; n < 9; n++) {
		regmask = 0;
		for(r = 0; r < 9; r++)
		if(s.m[r] & (1 << n))
			regmask |= 1 << (r / 3);
		if((rg = solved(regmask)) != 0)
			elregiov(m, n, rg - 1, c);
	}
}

void exhorizx(uint16_t m[81], int r)
{
	int n;
	ser s;
	int regmask;
	int rg, c, i;

	s = row(m, r);

	for(n = 0; n < 9; n++) {
		regmask = 0;
		for(c = 0; c < 9; c++)
		if(s.m[c] & (1 << n))
			regmask |= 1 << (r / 3);
		if((rg = solved(regmask)) != 0)
			elregiov(m, n, rg - 1, r);
	}
}

void elregiov(uint16_t m[81], int n, int rg, int xc)
{
	int cc, rr;
	int r, c;
	int i;

	r = (rg % 3) * 3;
	c = (rg / 3) * 3;

	for(rr = 0; rr < 3; rr++)
	for(cc = 0; cc < 3; cc++) {
		if(cc == xc % 3)
			continue;
		i = (r + rr) * 9 + cc + c;
		m[i] &= ~(1 << n);
	}
}

void elregioh(uint16_t m[81], int n, int rg, int xr)
{
	int cc, rr;
	int r, c;
	int i;

	r = (rg % 3) * 3;
	c = (rg / 3) * 3;

	for(rr = 0; rr < 3; rr++)
	for(cc = 0; cc < 3; cc++) {
		if(rr == xr % 3)
			continue;
		i = (r + rr) * 9 + cc + c;
		m[i] &= ~(1 << n);
	}
}

void elvertix(uint16_t m[81], int n, int c, int rg)
{
	int i, r;

	for(r = 0; r < 9; r++) {
		if(rg / 3 == r / 3) {
			continue;
		}
		i = r * 9 + c;
		m[i] &= ~(1 << n);
	}
}

void elhorizx(uint16_t m[81], int n, int r, int rg)
{
	int i, c;

	for(c = 0; c < 9; c++) {
		if(rg % 3 == c / 3) {
			continue;
		}
		i = r * 9 + c;
		m[i] &= ~(1 << n);
	}
}
