#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define COMPUTER 0
#define USER     1

static char mark[2] = {'O', 'X'};
static int turn;  // Player whose turn it is to decide on a move
static int n = 3;

typedef struct board {
    char marks[3*3];
    int last_move;
    int row_counts[2*3];  // First n values count O:s, rest X:s
    int col_counts[2*3];
    int diag_counts[2*2];
} board_t;

typedef struct score {
    int index;
    int value;
} score_t;

void make_move(board_t *board, int player, int index);
void undo_move(board_t *board, int index);

int winner(board_t *board, int player)
{
    int i = board->last_move / n;
    int j = board->last_move % n;

    if (board->row_counts[i + n*player] == n ||
        board->col_counts[j + n*player] == n)
        return 1;
    if (i == j && board->diag_counts[2*player] == n)
        return 1;
    if (i+j == n-1 && board->diag_counts[1 + 2*player] == n)
        return 1;
    return 0;
}

int max(int a, int b) { return a > b; }
int min(int a, int b) { return a < b; }

// Argument player is the conceived player at the current level of
// the tree. If player == turn, then the player wants to maximize
// the result.
score_t minimax(board_t *board, int player)
{
    score_t s, s_best;
    char *blanks;
    int index;
    int is_max = player == turn;
    int (*extremum)(int, int);

    if (winner(board, (player + 1) % 2)) {
        s.index = board->last_move;
        s.value = is_max ? 1 : -1;
        return s;
    }
    if ((blanks = strchr(board->marks, ' ')) == NULL) {
        s.index = -1;
        s.value = 0;
        return s;
    }

    if (is_max) {
        extremum = max;
        s_best.value = -10;
    } else {
        extremum = min;
        s_best.value = 10;
    }
    s_best.index = -1;  // Will be updated since at least one blank exists
    do {
        index = blanks - board->marks;
        make_move(board, player, index);
        s = minimax(board, (player + 1) % 2);
        if (extremum(s.value, s_best.value))
            s_best = s;
        undo_move(board, index);
    } while ((blanks = strchr(blanks, ' ')) != NULL);
    return s_best;
}

void make_move(board_t *board, int player, int index)
{
    int i = index / n;
    int j = index % n;
    printf("%d  (%d,%d)\n", index, i, j);
    sleep(1);

    board->marks[index] = mark[player];
    board->last_move = index;
    board->row_counts[i] += 1;
    board->col_counts[j] += 1;
    if (i == j)  // Main diagonal
        board->diag_counts[0] += 1;
    if (i+j == n-1)  // Opposite diagonal
        board->diag_counts[1] += 1;
}

void undo_move(board_t *board, int index)
{
    int i = index / n;
    int j = index % n;

    board->marks[index] = ' ';
    board->last_move = -1;
    board->row_counts[i] -= 1;
    board->col_counts[j] -= 1;
    if (i == j)
        board->diag_counts[0] -= 1;
    if (i+j == n-1)
        board->diag_counts[1] -= 1;
}

int main(int argc, char *argv[])
{
    board_t board;
    memset(board.marks, ' ', 3*3);
    board.last_move = -1;
    for (int i = 0; i < 2*n; i++)
        board.row_counts[i] = board.col_counts[i] = board.diag_counts[i] = 0;

    minimax(&board, COMPUTER);

    return 0;
}
