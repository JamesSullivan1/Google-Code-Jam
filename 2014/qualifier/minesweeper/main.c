/*
 * GOOGLE CODE JAM - 2014
 * Qualifier
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 * =====================================================================
 *
 * Problem
 *
 *  Minesweeper is a computer game that became popular in the 1980s, and
 *  is still included in some versions of the Microsoft Windows
 *  operating system. This problem has a similar idea, but it does not
 *  assume you have played Minesweeper.
 *
 *  In this problem, you are playing a game on a grid of identical
 *  cells. The content of each cell is initially hidden. There are M
 *  mines hidden in M different cells of the grid. No other cells
 *  contain mines. You may click on any cell to reveal it. If the
 *  revealed cell contains a mine, then the game is over, and you lose.
 *  Otherwise, the revealed cell will contain a digit between 0 and 8,
 *  inclusive, which corresponds to the number of neighboring cells that
 *  contain mines. Two cells are neighbors if they share a corner or an
 *  edge. Additionally, if the revealed cell contains a 0, then all of
 *  the neighbors of the revealed cell are automatically revealed as
 *  well, recursively. When all the cells that don't contain mines have
 *  been revealed, the game ends, and you win.
 *
 *  For example, an initial configuration of the board may look like
 *  this ('*' denotes a mine, and 'c' is the first clicked cell):
 *
 *  *..*...**.
 *  ....*.....
 *  ..c..*....
 *  ........*.
 *  ..........
 *
 *  There are no mines adjacent to the clicked cell, so when it is
 *  revealed, it becomes a 0, and its 8 adjacent cells are revealed as
 *  well. This process continues, resulting in the following board:
 *
 *  *..*...**.
 *  1112*.....
 *  00012*....
 *  00001111*.
 *  00000001..
 *
 *  At this point, there are still un-revealed cells that do not contain
 *  mines (denoted by '.' characters), so the player has to click again
 *  in order to continue the game.
 *
 *  You want to win the game as quickly as possible. There is nothing
 *  quicker than winning in one click. Given the size of the board (R x
 *  C) and the number of hidden mines M, is it possible (however
 *  unlikely) to win in one click? You may choose where you click. If it
 *  is possible, then print any valid mine configuration and the
 *  coordinates of your click, following the specifications in the
 *  Output section. Otherwise, print "Impossible". 
 *
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINE    -1
#define HIDDEN  -2
#define CLICK   -3

typedef struct coord {
    int x;
    int y;
} coord;

typedef struct board {
    int rows;
    int cols;
    int **elems;
} board;

void free_board(board *board)
{
    int i;
    if(!board)
        return;
    for(i = 0; i < board->rows; i++) {
        free(board->elems[i]);
    }
    free(board->elems);
    free(board);
}

/* For test purposes */
void print_boardt(board *board)
{
    int i, j;
    for(i = 0; i < board->rows; i++) {
        for(j = 0; j < board->cols; j++) {
            switch(board->elems[i][j]) {
                case MINE:
                    printf("*");
                    break;
                case CLICK:
                    printf("c");
                    break;
                case HIDDEN:
                    printf("."); 
                    break;
                default:
                    printf("%1d",board->elems[i][j]);
            } 
        }
        printf("\n");
    }
}

/* For use with output of solution */
void print_board(board *board)
{
    int i, j;
    for(i = 0; i < board->rows; i++) {
        for(j = 0; j < board->cols; j++) {
            switch(board->elems[i][j]) {
                case MINE:
                    printf("*");
                    break;
                case CLICK:
                    printf("c");
                    break;
                case HIDDEN:
                default:
                    printf("."); 
            } 
        }
        printf("\n");
    }
}

/* Returns a pointer to a row * col board, or NULL on failure. */
board *gen_board(int rows, int cols)
{
    board *ret = NULL;
    int i,j;

    if(rows < 0 || rows > 50)
        goto out;
    if(cols < 0 || cols > 50)
        goto out;

    ret = malloc(sizeof(board));
    if(!ret)
        goto out;
    ret->elems = malloc(rows * sizeof(int *));
    if(!ret->elems) {
        free(ret);
        goto out;
    }
    for(i = 0; i < rows; i++) {
        ret->elems[i] = malloc(cols * sizeof(int));
        if(!ret->elems[i]) {
            for(j = 0; j < i; j++) {
                free(ret->elems[j]);
            }
            free(ret->elems);
            free(ret);
            ret = NULL;
            goto out;
        }
    }
    ret->rows = rows;
    ret->cols = cols;

    for(i = 0; i < rows; i++)
        for(j = 0; j < cols; j++)
            ret->elems[i][j] = HIDDEN;

out:
    return ret;
}

/* Places the specified number of mines onto the board in such a way
 * that the bottom-right corner will be a solution to the click problem
 * just in case there is such a solution for this many mines. 
 *
 * The idea is to spread from the top left corner in a way that prevents
 * 'pockets' of unsolvable elements, such as:
 *
 *  Top right unsolvable    All solvable
 *  [ x x x - ]             [ x x - - ]
 *  [ - - - - ]     --->    [ x - - - ]
 *  [ - - - - ]             [ - - - - ]
 *  [ - - - c ]             [ - - - c ]
 *
 * Which is an element that has no zero-degree neighbours.
 */
int place_mines(board *board, coord loc, int mines)
{
    int i, ret = 1;
    coord next = { 0, 0 }; 

    if(!board)
        goto out;
    if(loc.x >= board->cols || loc.x < 0)
        goto out;
    if(loc.y >= board->rows || loc.y < 0)
        goto out;

    if(board->rows - loc.y >= board->cols - loc.x) {
        /* Special case when a pocket is made */ 
        if(board->cols - loc.x - mines == 1) {
            /* Fill until there are 2 empties left */
            i = loc.x;
            while(i < board->cols - 2 && mines-- > 0) {
                board->elems[loc.y][i++] = MINE;
            }
            loc.y++;
            /* Then switch to filling a column */
            goto fill_column;
        }
fill_row:
        i = loc.x;
        next.x = loc.x;
        next.y = loc.y + 1;
        while(i < board->cols && mines-- > 0) {
            board->elems[loc.y][i++] = MINE;
        }
    }
    else {
        /* Special case when a pocket is made */ 
        if(board->rows - loc.y - mines == 1) {
            /* Fill until there are 2 empties left */
            i = loc.y;
            while(i < board->rows - 2 && mines-- > 0) {
                board->elems[i++][loc.x] = MINE;
            }
            loc.x++;
            /* Then switch to filling a column */
            goto fill_row;
        }
fill_column:
        i = loc.y;
        next.x = loc.x + 1;
        next.y = loc.y;
        while(i < board->rows && mines-- > 0) {
            board->elems[i++][loc.x] = MINE;
        }
    }
    if(mines > 0)
        ret = place_mines(board, next, mines);
    else
        ret = 0;

out:
    return ret;
}

/* Returns 1 just in case the board is complete (no hidden squares
 * remain), and 0 otherwise.
 */
int complete(board *board)
{
    int i,j;
    int ret = 1;
    for(i = 0; i < board->rows; i++) {
        for(j = 0; j < board->cols; j++) {
            if(board->elems[i][j] == HIDDEN) {
                ret = 0;
                goto out;
            }
        }
    }
out:
    return ret;
}

/* Returns the degree of the square, ie the number of mines that
 * are adjacent to the square.
 * Returns -1 for an invalid coordinate.
 */
int degree(board *board, int x, int y)
{
    int ret = -1;
    int k, l;
    if(!board)
        goto out;
    if(y < 0 || y >= board->rows)
        goto out;
    if(x < 0 || x >= board->cols)
        goto out;

    if(board->elems[y][x] == MINE)
        goto out;

    ret = 0;
    for(k = -1; k <= 1; k++) {
        for(l = -1; l <= 1; l++) {
            if(k == 0 && l == 0) continue;
            if(x+k < 0 || y+l < 0) continue;
            if(x+k >= board->cols || y+l >= board->rows) continue;

            if(board->elems[y+l][x+k] == MINE)
                ret++;
        }  
    }

out:
    return ret;
}

/* Reveals the target square on the board.
 * If the square has degree 0, it also recursively reveals the
 * neighbours.
 */
int click(board *board, int x, int y)
{
    int ret = 1, recurse;
    int k, l;
    if(!board)
        goto out;
    if(y < 0 || y >= board->rows)
        goto out;
    if(x < 0 || x >= board->cols)
        goto out;

    if(board->elems[y][x] == MINE)
        goto out;

    /* Reveal the element */
    ret = 0;
    board->elems[y][x] = degree(board, x, y);
    if(board->elems[y][x] == 0)
        recurse = 1;
    else
        recurse = 0;
    /* Reveal the neighbours and possibly recurse */
    for(k = -1; k <= 1; k++) {
        for(l = -1; l <= 1; l++) {
            if(k == 0 && l == 0) continue;
            if(x+k < 0 || y+l < 0) continue;
            if(x+k >= board->cols || y+l >= board->rows) continue;

            if(board->elems[y+l][x+k] == HIDDEN && recurse)
                click(board, x+k, y+l);
        }  
    }
out:
    return ret;
}

/* Return a coordinate such that clicking on that square will
 * solve the board. On failure returns { -1, -1 }. 
 */
coord solve(board *board)
{
    int y, x;
    coord ret = { -1, -1 };
    if(!board)
        goto out;
    y = board->rows - 1;
    x = board->cols - 1;

    /* Find any 0 candidate- if one works, they all do */
    for(y = 0; y < board->rows; y++) {
        for(x = 0; x < board->cols; x++) {
            if(degree(board, x, y) == 0)
                goto click_test;
        }
    }
    /* If none are found, click the bottom right element */
    y--;
    x--;
click_test:
    /* Try to click here and see if it works */
    click(board, x, y);
    if(complete(board)) {
        ret.x = x;
        ret.y = y;
        board->elems[ret.y][ret.x] = CLICK;
    }

out:
    return ret; 
}

int main(void)
{
    int num_tests, test = 0, ret = -1;
    int rows, cols, mines;
    coord top_left = {0,0};
    coord solution = {-1,-1};
    board *board;
     
    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 230) {
        printf("Illegal number of tests (1 <= T <= 230)\n");
        goto out;
    }

    while(test++ < num_tests) {
        scanf("%d %d %d", &rows, &cols, &mines);

        board = gen_board(rows, cols);
        if(!board) {
            printf("Case #%d:\nImpossible\n",test);
            continue;
        }
        if(place_mines(board, top_left, mines)) {
            printf("Case #%d:\nImpossible\n",test);
            continue;
        }

        solution = solve(board);

        printf("Case #%d:\n",test);
        if(solution.x >= 0) {
            board->elems[solution.y][solution.x] = -3;
            print_board(board);
        }
        else {
            printf("Impossible\n");
        }
        free_board(board);
    }
   
    ret = 0;
out:
    return ret;
}

