#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "board.h"
#include "draw.h"

enum {
    COMPUTER = 0,
    USER = 1
};

static char psymbol[2] = {'O', 'X'};

// Just a data pair type used by minimax
typedef struct score {
    int index;
    int value;  // Only of interest internally in minimax
} score_t;

static int check_winner(board_t *board, int player);
static score_t minimax(board_t *board, int player);
static void make_move(board_t *board, int player, int index, int undo);
static void print_board(board_t *board);

// Returns 1 if player has a winning line in board, 0 if not. Note that only
// the row/column corresponding to the last move need to be checked.
static int check_winner(board_t *board, int player)
{
    int n = board->n;
    int i = board->last_move / n;
    int j = board->last_move % n;
    int pshift = n*player;

    // Check row where last move happened
    if (board->row_counts[i + pshift] == n)
        return 1;
    // Check column
    if (board->col_counts[j + pshift] == n)
        return 1;
    // Check main diagonal (if necessary)
    if (i == j && board->diag_counts[2*player] == n)
        return 1;
    // Check cross diagonal (if necessary)
    if (i+j == n-1 && board->diag_counts[1 + 2*player] == n)
        return 1;

    return 0;
}

static int max(int a, int b) { return a > b; }
static int min(int a, int b) { return a < b; }

// Argument player is the conceived player at the current level of
// the recursion tree. (COMPUTER is always the maximizing player.)
static score_t minimax(board_t *board, int player)
{
    score_t s, s_best;
    char *blanks;
    int index;
    int (*extremum)(int, int);

    if (check_winner(board, COMPUTER)) {
        s.value = 1;
        return s;
    }
    if (check_winner(board, USER)) {
        s.value = -1;
        return s;
    }
    if ((blanks = strchr(board->marks, ' ')) == NULL) {
        s.value = 0;
        return s;
    }

    if (player == COMPUTER) {
        extremum = max;
        s_best.value = -100;
    } else {
        extremum = min;
        s_best.value = 100;
    }
    do {
        index = blanks - board->marks;
        make_move(board, player, index, 0);
        s = minimax(board, (player + 1) % 2);
        if (extremum(s.value, s_best.value)) {
            s_best = s;
            s_best.index = index;
        }
        make_move(board, player, index, 1);
    } while ((blanks = strchr(blanks + 1, ' ')) != NULL);
    return s_best;
}

// Put mark corresponding to player at given index, or undo this operation if
// undo is true (thus decreasing row/column/diag counts again).
static void make_move(board_t *board, int player, int index, int undo)
{
    int n = board->n;
    int i = index / n;
    int j = index % n;
    int amount = undo ? -1 : 1;

    board->marks[index] = undo ? ' ' : psymbol[player];
    board->last_move = index;
    board->row_counts[i + n*player] += amount;
    board->col_counts[j + n*player] += amount;
    if (i == j)  // Main diagonal
        board->diag_counts[2*player] += amount;
    if (i+j == n-1)  // Opposite diagonal
        board->diag_counts[1 + 2*player] += amount;
}

// For debugging purposes
static void print_board(board_t *board)
{
    int n = board->n;
    for (int k = 0; k < n*n; k++) {
        if (k > 0 && k % n == 0)
            putchar('\n');
        printf("%2d: %-3c", k, board->marks[k]);
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    board_t *board = board_init(3);
    int c = 0;
    int curr_index = 0;
    WINDOW *win;

    make_move(board, USER, 0, 0);
    make_move(board, COMPUTER, 1, 0);
    make_move(board, USER, 2, 0);
    make_move(board, COMPUTER, 3, 0);
    make_move(board, COMPUTER, 4, 0);
    make_move(board, COMPUTER, 5, 0);
    make_move(board, USER, 6, 0);
    make_move(board, USER, 7, 0);
    make_move(board, USER, 8, 0);

    win = draw_init(board);
    do {
        switch (c) {
            case 'l':
                curr_index++;
                break;
        }
        draw(curr_index);
    } while ((c = wgetch(win)) != 'q');

    //make_move(board, USER, 0, 0);

    //score_t s = minimax(board, COMPUTER);
    //print_board(board); putchar('\n');
    //printf("s.index = %d\n", s.index);
    ////printf("s.value = %d\n", s.value);

    draw_free();
    board_free(board);
    return 0;
}
