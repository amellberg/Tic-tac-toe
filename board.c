#include <stdlib.h>
#include <string.h>
#include "board.h"

// TODO: error checking of malloc
board_t *board_create(int n)
{
    board_t *board;

    board = malloc(sizeof *board);
    board->n = n;
    board->marks = malloc(n*n + 1);
    board->row_counts = malloc(2*n * sizeof(int));
    board->col_counts = malloc(2*n * sizeof(int));

    memset(board->marks, ' ', n*n);
    board->marks[n*n] = '\0';
    memset(board->row_counts, 0, 2*n*sizeof(int));
    memset(board->col_counts, 0, 2*n*sizeof(int));
    memset(board->diag_counts, 0, 4*sizeof(int));

    return board;
}

void board_destroy(board_t *board)
{
    free(board->marks);
    free(board->row_counts);
    free(board->col_counts);
    free(board);
}
