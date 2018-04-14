#ifndef BOARD_H
#define BOARD_H

typedef struct board {
    char *marks;            // A board is a string of 'O', 'X' and ' ' chars
    int  n;                 // Of size n x n
    int  last_move;

    // Fields to track score of each player
    int  *row_counts;       // First n values count O:s (COMPUTER marks),
    int  *col_counts;       // the rest count X:s (USER marks)
    int  diag_counts[2*2];  // First two values are diagonal counts for
                            // COMPUTER, the other two for USER
} board_t;

board_t *board_create(int n);
void board_destroy(board_t *board);

#endif
