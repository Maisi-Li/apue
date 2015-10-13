OBJS = main.o helper.o print.o
CC = gcc
DEBUG = -g
COMPILE = -Wall -c $(DEBUG)
LINK = -Wall $(DEBUG)

all: $(OBJS) ls clean

ls: $(OBJS) 
	$(CC) $(LINK) $(OBJS) -o ls -lbsd -lm

main.o: main.c helper.h ls.h helper.c print.h print.c
	$(CC) $(COMPILE) main.c -g

helper.o: helper.c helper.h ls.h
	$(CC) $(COMPILE) helper.c

print.o: print.c print.h ls.h helper.h
	$(CC) $(COMPILE) print.c

.PHONY: clean

clean:
	rm *.o

tar: tar -cvf main.c
