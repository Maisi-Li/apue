OBJS = main.o
CC = gcc
DEBUG = -g
COMPILE = -Wall -c $(DEBUG)
LINK = -Wall $(DEBUG)


ls: $(OBJS) clean
	$(CC) $(LINK) $(OBJS) -o ls 

main.o: main.c
	$(CC) $(COMPILE) main.c

.PHONY: clean

clean:
	rm *.o

tar: tar -cvf main.c
