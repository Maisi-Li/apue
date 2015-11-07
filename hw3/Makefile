OBJS = main.o net.o
CC = gcc
DEBUG = -g
COMPILE = -Wall -c $(DEBUG)
LINK = -Wall $(DEBUG)

all: $(OBJS) sws clean

sws: $(OBJS)
	$(CC) $(LINK) $(OBJS) -o sws 

main.o: main.c sws.h
	$(CC) $(COMPILE) main.c
net.o: net.c sws.h
	$(CC) $(COMPILE) net.c

.PHONY: clean

clean:
	rm *.o

