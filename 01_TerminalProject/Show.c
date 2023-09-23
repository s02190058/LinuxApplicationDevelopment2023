#include <err.h>
#include <errno.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DY 2
#define DX 3

int get_number_of_digits(int n) {
    int res = 0;
    while (n) {
        n /= 10;
        ++res;
    }
    return res;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: Show FILE\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        err(EXIT_FAILURE, "%s", filename);
    }

    int number_of_lines = 0;
    size_t size = 16;
    char **lines = (char **) calloc(size, sizeof (char *));
    if (!lines) {
        fclose(file);
        err(EXIT_FAILURE, "calloc");
    }

    int exit_code = EXIT_SUCCESS;

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != EOF) {
        if ((size_t) number_of_lines == size) {
            size *= 2;
            char **tmp = (char **) realloc(lines, sizeof (char *) * size);
            if (!tmp) {
                fprintf(stderr, "%s: realloc(): %s\n", argv[0], strerror(errno));
                exit_code = EXIT_FAILURE;
                goto clean_up;
            }
            lines = tmp;
        }

        lines[number_of_lines] = line;
        ++number_of_lines;
        line = NULL;
        len = 0;
    }

    if (!feof(file)) {
        fprintf(stderr, "%s: getline(): %s\n", argv[0], strerror(errno));
        exit_code = EXIT_FAILURE;
        goto clean_up;
    }

    initscr();
    noecho();
    cbreak();
    printw("File: %s; size: %d\n", filename, number_of_lines);
    refresh();

    int nlines = LINES - 2 * DY;
    int ncols = COLS - 2 * DX;
    WINDOW *win = newwin(nlines, ncols, DY, DX);
    keypad(win, TRUE);

    int offset = 0;
    int c = 0;
    do {
        switch (c) {
            case ' ':
            case KEY_DOWN:
                if (offset + nlines - 2 < number_of_lines) {
                    ++offset;
                }
                break;
            case KEY_UP:
                if (offset > 0) {
                    --offset;
                }
                break;
        }

        for (int i = 0; i < nlines - 2 && i < number_of_lines; ++i) {
            int number_of_digits = get_number_of_digits(number_of_lines);
            int str_max_length = ncols - number_of_digits - 4;
            mvwprintw(win, i + 1, 2, "%*d %.*s\n", number_of_digits,
                      offset + i + 1, str_max_length, lines[offset + i]);
        }
        box(win, 0, 0);
        wrefresh(win);
    } while ((c = wgetch(win)) != 27);

    endwin();

clean_up:
    fclose(file);
    for (int i = 0; i < number_of_lines; ++i) {
        free(lines[i]);
    }
    free(lines);

    return exit_code;
}
