CFLAGS= -Wall -Wextra -std=c11 -pedantic -ggdb

heap: my_alloc.c
	$(CC) $(CFLAGS) -o heap my_alloc.c
