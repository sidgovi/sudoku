CC=gcc
FLAGS=-O2
LIBS=-lm
sudoku-solver: sudoku.c
	${CC} ${FLAGS} ${LIBS} -o sudoku-solver sudoku.c
