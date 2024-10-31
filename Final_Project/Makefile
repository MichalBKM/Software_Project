CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm

.PHONY: clean

symnmf: symnmf.o
	$(CC) -o $@ $^ $(LDFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) -c $< $(CFLAGS)

symnmfmodule.o: symnmfmodule.c symnmf.h
	$(CC) -shared -o symnmf.so $< $(shell python3.12-config --includes --ldflags)

clean:
	rm -f *.o symnmf symnmf.so
