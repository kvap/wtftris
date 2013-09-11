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
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "field.h"
#include "figure.h"
#include "shapes.h"

void finish(int sig) {
	curs_set(1);
	endwin();
	printf("retcode = %d\n", sig);
	exit(sig);
}

int *shapes[] = {
	shape_o,
	shape_s,
	shape_z,
	shape_l,
	shape_j,
	shape_t,
	shape_i
};
int shapes_count = sizeof(shapes) / sizeof(shapes[0]);

int stats[7];

figure_t gen_figure(int y, int x) {
	figure_t figure;
	figure.x = x;
	figure.y = y;

	int choice = rand() % shapes_count;
	stats[choice]++; 
	figure.shape = shapes[choice];

	figure.orientation = 0;
	return figure;
}

#define FIELD_HEI 20
#define FIELD_WID 10

float now_s() {
	// current time in seconds
	struct timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) == 0) {
		return t.tv_sec + t.tv_nsec * 1e-9;
	} else {
		printf("Error while clock_gettime()\n");
		finish(-1);
		return 0;
	}
}

void init_stats() {
	for (int i = 0; i < sizeof(stats) / sizeof(stats[0]); i++) {
		stats[i] = 0;
	}
}

void show_stats() {
	for (int i = 0; i < sizeof(stats) / sizeof(stats[0]); i++) {
		attron(COLOR_PAIR(i + 1));
		mvprintw(4 + i, 1, "%d      ", stats[i]);
		attron(COLOR_PAIR(1));
	}
}

#define SCORES_FILENAME "wtftris.save"
int get_hiscore() {
	FILE *f = fopen(SCORES_FILENAME, "r");
	if (!f) {
		return 0;
	}
	int score;
	int highest = 0;
	while (fscanf(f, "%d\n", &score) != EOF) {
		if (score > highest) {
			highest = score;
		}
	}
	fclose(f);
	return highest;
}

void put_hiscore(int score) {
	FILE *f = fopen(SCORES_FILENAME, "a");
	if (f) {
		fprintf(f, "%d\n", score);
		fclose(f);
	}
}

int main() {
	signal(SIGINT, finish);

	srand(time(NULL));

	initscr();
	keypad(stdscr, TRUE);
	nonl();
	noecho();
	cbreak();
	curs_set(0);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	init_stats();

	field_t field = create_field(FIELD_HEI, FIELD_WID);
	figure_t figure = gen_figure(0, FIELD_WID/2 - 2);

	int score = 0;
	int hiscore = get_hiscore();

	int gameover = 0;
	float t = now_s();
	while (1) {
		usleep(100);
		if (gameover) {
			put_hiscore(score);

			if (score > hiscore) {
				hiscore = score;
			}

			nodelay(stdscr, FALSE);
			mvprintw(1, 1, "game over, press any key to restart or 'q' to exit");
			refresh();
			int c = getch();

			t = now_s();
			if (c == 'q') {
				finish(-c);
			} else {
				clear_field(field);
				score = 0;
				gameover = 0;
				continue;
			}
		}
		nodelay(stdscr, TRUE);

		float dt = now_s() - t;
		int c = getch();
		switch (c) {
			case 'q':
				finish(-c);
			case KEY_UP:
				if (move_figure(field, &figure, 0, 0, 1)) {
					score--;
				}
				break;
			case KEY_LEFT:
				if (move_figure(field, &figure, -1, 0, 0)) {
					score--;
				}
				break;
			case KEY_RIGHT:
				if (move_figure(field, &figure, 1, 0, 0)) {
					score--;
				}
				break;
			case KEY_DOWN:
				score++;
				dt = 1.1;
				break;
		}
		if (dt > 1) {
			if (!move_figure(field, &figure, 0, 1, 0)) {
				brand_figure(field, figure);
				int bonus = burn_field(field);
				bonus = round(pow(10, bonus));
				if (field_is_empty(field)) {
					bonus *= 1000;
				}
				score += bonus;

				figure = gen_figure(0, FIELD_WID/2 - 2);
				if (figure_hits_field(field, figure)) {
					gameover = 1;
					continue;
				}
			}
			t = now_s();
		}

		draw_field(field);
		draw_figure(field, figure);

		if (score > hiscore) {
			mvprintw(2, 1, "score: %d (record)        ", score);
		} else {
			mvprintw(2, 1, "score: %d < hiscore: %d        ", score, hiscore);
		}

		show_stats();

		refresh();
	}

	return 0;
}
