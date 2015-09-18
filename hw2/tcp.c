#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
	int inputFd, outputFd;
	DIR *outputDir;
	ssize_t readIn, WriteOut;
	char buffer[BUFFER_SIZE];
	struct stat stat1, stat2;
	if(argc != 3) {
		fprintf(stderr, "tcp: Usage: %s filename filename\n",argv[0]);
		exit(1);
	}

	// Create input file descroptor
	inputFd = open(argv[1],O_RDONLY);
	if(inputFd == -1) {
		fprintf(stderr,"%s: %s: %s\n",
				argv[0], argv[1], strerror(errno));
		exit(2);
	}
	if(stat(argv[1],&stat1) == -1) {
		fprintf(stderr, "%s: stat: %s",argv[0], strerror(errno));
	}
	printf("stat1.inode = %d\n", (int)stat1.st_ino);

	// Check if the second argument is a file or a Directory 	
	if(stat(argv[2],&stat2) == -1)	{
		fprintf(stderr,"%s: %s: %s",argv[0], argv[2],strerror(errno));
		exit(3);
	}
	
	if(S_ISDIR(stat2.st_mode)) 
		outputDir = opendir(argv[2]);	
	else
		outputFd = open(argv[2],O_WRONLY);
	if(outputFd != -1)
		printf("stat2.inode = %d\n", (int)stat2.st_ino);
	return(0);
}
