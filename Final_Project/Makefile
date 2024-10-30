CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -lm

.PHONY: clean

symnmf: symnmf.o symnmf.h
	$(CC) -o symnmf symnmf.o symnmf.h $(CFLAGS)
	rm -f symnmf.o
symnmf.o: symnmf.c
	$(CC) -c symnmf.c $(CFLAGS)

clean:
	rm -f symnmf
