#include <ncurses.h>
#include "draw.h"
#include "board.h"

enum {
    CELL_WIDTH = 5
};

static board_t *board;
static int n;
static WINDOW *win;
static int curr_cell;  // Index of highlighted cell, between 0 and n-1

static void draw_frame(void)
{
    int y, x;

    waddch(win, ACS_ULCORNER);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < CELL_WIDTH; j++)
            waddch(win, ACS_HLINE);
        waddch(win, ACS_TTEE);
    }
    mvwaddch(win, 0, n*CELL_WIDTH + n, ACS_URCORNER);  // Replace end ACS_TTEE

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            getyx(win, y, x);
            waddch(win, ACS_VLINE);
            wmove(win, y, x + CELL_WIDTH + 1);
        }
        waddch(win, ACS_LTEE);
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < CELL_WIDTH; k++)
                waddch(win, ACS_HLINE);
            waddch(win, ACS_PLUS);
        }
        getyx(win, y, x);
        mvwaddch(win, y-1, n*CELL_WIDTH + n, ACS_RTEE);
    }

    getyx(win, y, x);
    mvwaddch(win, y-1, 0, ACS_LLCORNER);
    for (int i = 0; i < n; i++) {
        getyx(win, y, x);
        mvwaddch(win, y, x + CELL_WIDTH, ACS_BTEE);
    }
    mvwaddch(win, y, x + CELL_WIDTH, ACS_LRCORNER);
}

WINDOW *draw_init(board_t *b)
{
    initscr();
    noecho();
    curs_set(0);

    board = b;
    n = board->n;
    win = newwin(2*n + 2, n*CELL_WIDTH + (n+1), 3, 8);
    draw_frame();

    return win;
}

void draw_free(void)
{
    endwin();
}

void draw(void)
{
    wrefresh(win);
    wgetch(win);
}
