OBJS = main.o helper.o
CC = gcc
DEBUG = -g
COMPILE = -Wall -c $(DEBUG)
LINK = -Wall $(DEBUG)

all: $(OBJS) ls clean

ls: $(OBJS) 
	$(CC) $(LINK) $(OBJS) -o ls -lbsd

main.o: main.c helper.h ls.h helper.c
	$(CC) $(COMPILE) main.c 

helper.o: helper.c helper.h ls.h
	$(CC) $(COMPILE) helper.c
.PHONY: clean

clean:
	rm *.o

tar: tar -cvf main.c
