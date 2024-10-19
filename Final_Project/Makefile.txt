CC = gcc
CFLAGS = -ansi  -Wall -Wextra -Werror -pedantic-errors

.PHONY: clean

symnmf: symnmf.o
	$(CC) -o symnmf symnmf.o $(CFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) -c symnmf.c $(CFLAGS)

clean:
	rm -f symnmf symnmf.o
