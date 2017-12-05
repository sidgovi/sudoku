#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GIT_COLOR_RESET		"\033[m"
#define GIT_COLOR_GREEN		"\033[32m"

const char usage_str[] = "usage: sudoku-solver <PUZZLE>\n\n"
			 "./sudoku-solver 300200000000107000706030500070009080900020004010800050009040301000702000000008006\n\n"
		     	 "The PUZZLE is a string of 81 digits\n"
		     	 "with 0's representing the unknown numbers.";

static int n_row[9];
static int n_col[9];
static int n_square[9];

static int get_square(int row, int col)
{
	return col/3 + 3*(row/3); 
}

static void print_board(int board[9][9])
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			printf("%s%2d%s%s", board[i][j]< 0 ? GIT_COLOR_GREEN : "", 
				abs(board[i][j]), j == 8 ? "\n": "", GIT_COLOR_RESET);
}

static int solve_board(int board[9][9], int *n_row, int *n_col, int *n_square, int row, int col)
{
	int k;

	if (board[row][col] > 0) {
		if (row == 8 && col == 8)
			return 0;
		return solve_board(board, n_row, n_col, n_square, 
					col == 8 ? row+1 : row, col == 8 ? 0 : col+1);
	} else {
		for (k = 1; k <= 9; ++k) {
			if ((n_row[row] & (1 << k)) \
				|| (n_col[col] & (1 << k)) 
				|| (n_square[get_square(row,col)] & (1 << k))) 
				continue;
			else {
				board[row][col] =  -1 *k;
				n_row[row] |= (1 << k);
				n_col[col] |= (1 << k);
				n_square[get_square(row,col)] |= (1 << k);
				if (row == 8 && col == 8)
					return 0;
				if (solve_board(board, n_row, n_col, n_square, col == 8 ? row+1 : row, 
							col == 8 ? 0 : col + 1) == -1) {
					n_row[row] &= ~(1 << k);
					n_col[col] &= ~(1 << k);
					n_square[get_square(row,col)] &= ~(1 << k);
					board[row][col] = 0;
				} else
					break;
			}
		}
		if (k == 10)
			return -1;

	}
	return 0;
}

static void init_avail(int board[9][9], int *n_row, int *n_col, int *n_square)
{
        for (int i = 0; i < 9; ++i) {
                n_row[i] = 0;
                n_col[i] = 0;
		n_square[i] = 0;
        }

	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j)
			if (board[i][j] > 0) {
				n_row[i] |= (1 << board[i][j]);
				n_col[j] |= (1 << board[i][j]);
				n_square[get_square(i,j)] |= (1 << board[i][j]);
			}
}

static void usage()
{
	printf("%s\n\n", usage_str); 
	
	exit(128);
}

int main(int argc, char *argv[])
{      

	static int board[9][9];

	if (argc != 2 || strlen(argv[1]) != 81) {
		usage();
	}

	for (int i = 0; i < 81; ++i) {
		char c = argv[1][i];
		int n = c - '0';
		if (n < 0 || n > 9) {
			printf("Invalid character: '%c'\n", c);
			usage();
		}
		board[i/9][i-9*(i/9)] = n;
	}

	init_avail(board, n_row, n_col, n_square);
	solve_board(board, n_row, n_col, n_square, 0, 0);
	print_board(board);
}
