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
static int n;  // Board is n x n in size
static int curr_index = 0;  // Currently selected board cell

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

int get_move(board_t *board, int turn, WINDOW *win)
{
    if (turn == COMPUTER) {
        score_t s = minimax(board, COMPUTER);
        return s.index;
    } else {  // USER makes a move
        int c;
        while ((c = wgetch(win)) != 'q') {
            switch (c) {
                case KEY_LEFT:
                case 'h':
                    if ((curr_index % n != 0) && curr_index > 0)
                        curr_index--;
                    break;
                case KEY_RIGHT:
                case 'l':
                    if ((curr_index % n != (n-1)) && curr_index < n*n - 1)
                        curr_index++;
                    break;
                case KEY_UP:
                case 'k':
                    if (curr_index >= n)
                        curr_index -= n;
                    break;
                case KEY_DOWN:
                case 'j':
                    if (curr_index < n*n - n)
                        curr_index += n;
                    break;
                case ' ':
                    if (board->marks[curr_index] == ' ')
                        return curr_index;
                    break;
            }
            draw(curr_index);
        }
        return -1;
    }
}


int main(int argc, char *argv[])
{
    board_t *board;
    WINDOW *win;
    int turn;         // Current player to make a move (COMPUTER or USER)
    int game_result;  // 0 or 1 for COMPUTER or USER win, 2 for draw
    int index;

    curr_index = 0;
    n = 3;
    board = board_init(n);
    win = draw_init(board);

    turn = USER;
    game_result = 2;
    draw(curr_index);
    for (int i = 0; i < n*n; i++) {
        index = get_move(board, turn, win);
        if (index == -1) break;  // User pressed 'q'
        make_move(board, turn, index, 0);
        draw(curr_index);
        if (check_winner(board, turn)) {
            game_result = turn;
            break;
        }
        turn = (turn + 1) % 2;
    }
    draw_free();
    board_free(board);
    //mvprintw(30, 10, "%d", game_result);
    //refresh(); getch();

    // TODO: add in-game printing of result; let user see final board
    if (game_result == COMPUTER) {
        printf("You lose!\n");
    } else if (game_result == USER) {
        printf("You win!\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}
