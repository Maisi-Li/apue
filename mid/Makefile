OBJS = main.o helper.o
CC = gcc
DEBUG = -g
COMPILE = -Wall -c $(DEBUG)
LINK = -Wall $(DEBUG)


ls: $(OBJS) 
	$(CC) $(LINK) $(OBJS) -o ls 

main.o: main.c helper.h ls.h helper.c
	$(CC) $(COMPILE) main.c

helper.o: helper.c helper.h ls.h
	$(CC) $(COMPILE) helper.c
.PHONY: clean

clean:
	rm *.o

tar: tar -cvf main.c
