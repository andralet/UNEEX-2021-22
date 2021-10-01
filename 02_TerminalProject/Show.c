#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <curses.h>

enum { SIDE = 3 };
#define ROWS_IN_WINDOW (LINES - 2 * SIDE - 2)
#define COLS_IN_WINDOW (COLS - 2 * SIDE - 2)
enum { KEY_ESCAPE_CODE = 27 };

typedef struct line {
	char *data;
	size_t len;
} Line;

Line *load_lines(const char *filename, size_t *lines_num, size_t *max_len);
void free_lines(Line *lines, size_t lines_num);
void process_key(int key, size_t *row, size_t *column, size_t row_num, size_t max_len);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <file to show>\n", argv[0]);
		return 1;
	}

	size_t row_num = 0, max_len = 0;
	Line *lines = load_lines(argv[1], &row_num, &max_len);
	if (!lines) {
		return 2;
	}
	size_t row = 0, column = 0;

	initscr(); noecho(); cbreak();
	curs_set(0); // hiding cursor
	WINDOW *win = newwin(LINES - 2 * SIDE, COLS - 2 * SIDE, SIDE, SIDE);
	keypad(win, TRUE);

	int key = 0;
	do {
		process_key(key, &row, &column, row_num, max_len);
		erase(); // clearing screen
		werase(win);
		move(0, 0);
		printw("%s %d:%d (%d%%)", argv[1], row + 1, column, (row + ROWS_IN_WINDOW) * 100 / row_num);
		wprintw(win, "\n"); // skipping box row
		for (size_t cur_row = row; cur_row < row + ROWS_IN_WINDOW; ++cur_row) {
			if (column <= lines[cur_row].len) {
				wprintw(win, " %3d: %-.*s\n", cur_row + 1, COLS_IN_WINDOW - 5, lines[cur_row].data + column);
			} else {
				wprintw(win, " %3d:\n", cur_row + 1);
			}
		}
		box(win, 0, 0);
		refresh();
	} while((key = wgetch(win)) != KEY_F(1) && key != 'q' && key != 'Q' && key != KEY_ESCAPE_CODE);
	free_lines(lines, row_num);
	endwin();

	return 0;
}

Line *load_lines(const char *filename, size_t *lines_num, size_t *max_len) {
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		fprintf(stderr, "Show: Error: couldn't open file '%s'.\n", filename);
		return NULL;
	}

	*max_len = 0;
	enum { DEFAULT_LINES_SIZE = 1024, MUL = 2 };
	*lines_num = DEFAULT_LINES_SIZE;
	Line *lines = calloc(*lines_num, sizeof(Line));
	if (!lines) {
		fprintf(stderr, "Show: Error: not enough memory.\n");
		return NULL;
	}

	size_t len = 0;
	size_t line_id = 0;
	ssize_t delimiter_position = 0;
	errno = 0; // looking up for errors from getline()
	while ((delimiter_position = getline(&(lines[line_id].data), &len, fin) - 1) >= 0) {
		lines[line_id].data[delimiter_position] = '\0';
		lines[line_id].len = delimiter_position;
		if ((size_t)delimiter_position > *max_len) {
			*max_len = (size_t)delimiter_position;
		}
		line_id++;
		if (line_id >= *lines_num) {
			*lines_num *= MUL;
			Line *temp = realloc(lines, *lines_num * sizeof(Line));
			if (!temp) {
				fprintf(stderr, "Show: Error: not enough memory.\n");
				free_lines(lines, line_id);
				return NULL;
			}
			memset(lines + line_id, 0, (*lines_num - line_id) * sizeof(Line));
		}
		errno = 0;
		len = 0; // for next getline()
	}
	fclose(fin);
	if (delimiter_position <= -2) { // -1 stands for just 0 bytes in string (probably, right before EOF, but not an error for sure)
		if (errno == EINVAL) {
			fprintf(stderr, "Show: Internal error on loading lines. Contact the maintainer.\n");
			free_lines(lines, line_id);
			return NULL;
		} else if (errno == ENOMEM) {
			fprintf(stderr, "Show: Error: not enough memory.\n");
			free_lines(lines, line_id);
			return NULL;
		}
		// else it's just EOF
	}

	*lines_num = line_id; // we don't need extra lines.
	Line *temp = realloc(lines, line_id * sizeof(Line));
	if (temp) { // check just for case realloc suddenly fails
		lines = temp;
	}
	return lines;
}

void free_lines(Line *lines, size_t lines_num) {
	for (size_t i = 0; i < lines_num; ++i) {
		free(lines[i].data);
	}
	free(lines);
}

void process_key(int key, size_t *row, size_t *column, size_t row_num, size_t max_len) {
	switch (key) {
		case KEY_UP: case 'k': case 'K':
			if (*row > 0) {
				(*row)--;
			}
			break;
		case KEY_PPAGE:
			if (*row >= (size_t)ROWS_IN_WINDOW) {
				*row -= ROWS_IN_WINDOW;
			} else {
				*row = 0;
			}
			break;
		case 'g':
			*row = 0;
			break;
		case KEY_DOWN: case 'j': case 'J': case ' ':
			if (*row + ROWS_IN_WINDOW < row_num) {
				(*row)++;
			}
			break;
		case KEY_NPAGE:
			if (*row + 2 * ROWS_IN_WINDOW < row_num) {
				*row += ROWS_IN_WINDOW;
			} else {
				*row = row_num - ROWS_IN_WINDOW;
			}
			break;
		case 'G':
			*row = row_num - ROWS_IN_WINDOW;
			break;

		case KEY_LEFT: case 'h': case 'H':
			if (*column > 0) {
				(*column)--;
			}
			break;
		case KEY_HOME:
			*column = 0;
			break;
		case KEY_RIGHT: case 'l': case 'L':
			if (*column < max_len - 1) {
				(*column)++;
			}
			break;
		case KEY_END:
			*column = max_len - 1;
			break;
	}
}

