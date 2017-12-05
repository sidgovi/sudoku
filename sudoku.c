#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GIT_COLOR_RESET		"\033[m"
#define GIT_COLOR_GREEN		"\033[32m"

const char usage_str[] = 	"\nsudoku-solver <PUZZLE>\n"
		     		"The PUZZLE is a string of 81 digits\n"
		     		"with 0's representing the unknown numbers.";

static int avail_by_row[9];
static int avail_by_col[9];
static int avail_by_square[9];

static int get_square(int row, int col)
{
	return col/3 + 3*(row/3); 
}

static void init()
{
        for (int i = 0; i < 9; ++i) {
                avail_by_row[i] = 0;
                avail_by_col[i] = 0;
		avail_by_square[i] = 0;
        }
}

static void print_board(int board[9][9])
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			printf("%s%2d%s%s", board[i][j]< 0 ? GIT_COLOR_GREEN : "", abs(board[i][j]), j == 8 ? "\n": "", GIT_COLOR_RESET);
}

static int fill_board(int board[9][9], int *avail_by_row, int *avail_by_col, int *avail_by_square, int row, int col)
{
	int k;
	//print_board(board);
	if (board[row][col] > 0) {
		if (row == 8 && col == 8)
			return 0;
		return fill_board(board, avail_by_row, avail_by_col, avail_by_square, col == 8 ? row+1 : row, col == 8 ? 0 : col+1);
	} else {
		for (k = 1; k <= 9; ++k) {
			if ((avail_by_row[row] & (1 << k)) \
				|| (avail_by_col[col] & (1 << k)) 
				|| (avail_by_square[get_square(row,col)] & (1 << k))) 
				continue;
			else {
				board[row][col] =  -1 *k;
				avail_by_row[row] |= (1 << k);
				avail_by_col[col] |= (1 << k);
				avail_by_square[get_square(row,col)] |= (1 << k);
				if (row == 8 && col == 8)
					return 0;
				if (fill_board(board, avail_by_row, avail_by_col, avail_by_square, col == 8 ? row+1 : row, col == 8 ? 0 : col + 1) == -1) {
					avail_by_row[row] &= ~(1 << k);
					avail_by_col[col] &= ~(1 << k);
					avail_by_square[get_square(row,col)] &= ~(1 << k);
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

static void init_avail(int board[9][9], int *avail_by_row, int *avail_by_col, int *avail_by_square)
{
	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j)
			if (board[i][j] > 0) {
				avail_by_row[i] |= (1 << board[i][j]);
				avail_by_col[j] |= (1 << board[i][j]);
				avail_by_square[get_square(i,j)] |= (1 << board[i][j]);
			}
}

static void usage()
{
	fprintf(stderr, "fatal: Invalid board\n");
	printf("%s\n\n", usage_str); 
	
	exit(128);
}

int main(int argc, char *argv[])
{      

	static int board[9][9];

	init();

	if (strlen(argv[1]) != 81) {
		printf("Board length != 81\n");
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
	init_avail(board, avail_by_row, avail_by_col, avail_by_square);
	fill_board(board, avail_by_row, avail_by_col, avail_by_square, 0, 0);
	print_board(board);
}
