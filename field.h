/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef FIELD_H
#define FIELD_H

typedef struct field_t {
	int wid, hei;
	int *occupied;
} field_t;

field_t create_field(int hei, int wid);
void clear_field(field_t f);
int occupied(field_t f, int row, int col);
void brand(field_t f, int row, int col, int color);
void draw_field(field_t f);
int burn_field(field_t f);
int field_is_empty(field_t f);

#endif
