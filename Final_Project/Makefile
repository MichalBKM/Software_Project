CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm

.PHONY: clean

symnmf: symnmf.o
	$(CC) -o $@ $^ $(LDFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o symnmf symnmf.so
