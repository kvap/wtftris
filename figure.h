/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef FIGURE_H
#define FIGURE_H

#include "field.h"
#include "shapes.h"

typedef struct figure_t {
	int x, y;
	int orientation;
	int *shape;
} figure_t;

void draw_figure(field_t field, figure_t figure);
int figure_hits_field(field_t field, figure_t figure);
int move_figure(field_t field, figure_t *figure, int dx, int dy, int dorient);
void brand_figure(field_t field, figure_t figure);

#endif
