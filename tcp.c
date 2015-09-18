#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
	int inputFd, outputFd, outputDir;
	ssize_t readIn, WriteOut;
	char buffer[BUFFER_SIZE];

	if(argc != 3) {
		fprintf(stderr, "tcp: Usage: %s filename filename\n",argv[0]);
		exit(1);
	}

	// Create input file descroptor
	inputFd = open(argv[1],O_RDONLY);
	if(inputFd == -1) {
		fprintf(stderr,"tcp: can not open file1: %s\n",
				strerror(errno));
		exit(2);
	}

	return(2);
}
