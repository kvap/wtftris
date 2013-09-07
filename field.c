/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "field.h"

field_t create_field(int hei, int wid) {
	field_t f;
	f.wid = wid;
	f.hei = hei;
	f.occupied = malloc(wid * hei * sizeof(int));
	memset(f.occupied, 0, wid * hei * sizeof(int));
	return f;
}

void clear_field(field_t f) {
	memset(f.occupied, 0, f.wid * f.hei * sizeof(int));
}

int full_row(field_t f, int row) {
	for (int col = 0; col < f.wid; col++) {
		if (!occupied(f, row, col)) {
			return 0;
		}
	}
	return 1;
}

void shift(field_t f, int row) {
	memmove(f.occupied + f.wid, f.occupied, row * f.wid * sizeof(int));
	memset(f.occupied, 0, f.wid * sizeof(int));
}

int field_is_empty(field_t f) {
	for (int row = 0; row < f.hei; row++) {
		for (int col = 0; col < f.wid; col++) {
			if (occupied(f, row, col)) {
				return 0;
			}
		}
	}
	return 1;
}

int burn_field(field_t f) {
	int burnt = 0;
	for (int row = 1; row < f.hei; row++) {
		if (full_row(f, row)) {
			shift(f, row);
			burnt++;
		}
	}
	return burnt;
}

int occupied(field_t f, int row, int col) {
	if ((row >= f.hei) || (col < 0) || (col >= f.wid)) {
		return 1;
	}
	if (row < 0) {
		return 0;
	}
	return f.occupied[row * f.wid + col];
}

void brand(field_t f, int row, int col) {
	if ((row < 0) || (row >= f.hei) || (col < 0) || (col >= f.wid)) {
		return;
	}
	f.occupied[row * f.wid + col] = 1;
}

void draw_field(field_t f) {
	int yorigin = LINES/2 - f.hei/2;
	int xorigin = COLS/2 - f.wid/2;

	// top
	move(yorigin - 1, xorigin - 1);
	addch('+');
	for (int col = 0; col < f.wid; col++) {
		addch('-');
	}
	addch('+');

	// middle
	for (int row = 0; row < f.hei; row++) {
		move(yorigin + row, xorigin - 1);
		addch('|');
		for (int col = 0; col < f.wid; col++) {
			if (occupied(f, row, col)) {
				addch('#');
			} else {
				addch(' ');
			}
		}
		addch('|');
	}

	// bottom
	move(yorigin + f.hei, xorigin - 1);
	addch('+');
	for (int col = 0; col < f.wid; col++) {
		addch('-');
	}
	addch('+');
}
