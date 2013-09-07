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
#include "figure.h"

void draw_figure(field_t field, figure_t figure) {
	int yorigin = LINES/2 - field.hei/2;
	int xorigin = COLS/2 - field.wid/2;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			if (shape_test(figure.shape, figure.orientation, row, col)) {
				mvaddch(
					yorigin + figure.y + row,
					xorigin + figure.x + col,
					'%'
				);
			}
		}
	}
}

int figure_hits_field(field_t field, figure_t figure) {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			int shape_is_here = shape_test(
				figure.shape,
				figure.orientation,
				row, col
			);
			int field_is_here = occupied(
				field,
				figure.y + row,
				figure.x + col
			);
			if (shape_is_here && field_is_here) {
				return 1;
			}
		}
	}
	return 0;
}

int move_figure(field_t field, figure_t *figure, int dx, int dy, int dorient) {
	// put the figure into the new position
	figure->x += dx;
	figure->y += dy;
	figure->orientation = (figure->orientation + 4 + dorient) % 4;

	if (figure_hits_field(field, *figure)) {
		// revert the old position
		figure->x -= dx;
		figure->y -= dy;
		figure->orientation = (figure->orientation + 4 - dorient) % 4;
		return 0;
	}

	return 1;
}

void brand_figure(field_t field, figure_t figure) {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			int shape_is_here = shape_test(
				figure.shape,
				figure.orientation,
				row, col
			);
			if (shape_is_here) {
				brand(
					field,
					figure.y + row,
					figure.x + col
				);
			}
		}
	}
}
