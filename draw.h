#ifndef DRAW_H
#define DRAW_H
#include "board.h"

WINDOW *draw_init(board_t *board);
void draw_free(void);

void draw(void);


#endif
