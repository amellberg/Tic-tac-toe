#include <ncurses.h>
#include "draw.h"
#include "board.h"

static board_t *board;
static WINDOW *win;

static void draw_frame(void)
{
    box(win, 0, 0);
}

WINDOW *draw_init(board_t *b)
{
    initscr();
    noecho();
    curs_set(0);

    board = b;
    win = newwin(2*board->n + 1, 5*board->n, 3, 8);
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
