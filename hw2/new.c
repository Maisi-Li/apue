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
void copyDir(char *file, char *dir);
void copyFile(char *file1, char *file2);
void getFullPath(char *file, char *dir, char *temp);


int main(int argc, char* argv[]) {
	int inputFd, outputFd;
	DIR *outputDir;
	ssize_t readIn, writeOut;
	char buffer[BUFFER_SIZE];
	char *temp = (char*) malloc(4096 *sizeof(char));
	struct stat stat1, stat2,stat_temp;
	// only two argument is accepted
	if(argc != 3) {
		fprintf(stderr, "tcp: Usage: %s filename filename\n",argv[0]);
		exit(1);
	}
	// State of the arg1
	if(stat(argv[1],&stat1) == -1) {
		fprintf(stderr, "%s: %s: %s\n",argv[0], argv[1], strerror(errno));
		exit(2);
	}
	
	//State of the arg2
	if(stat(argv[2],&stat2) == -1)	{
		fprintf(stderr,"%s: %s: %s\n", argv[0], argv[2], strerror(errno));
		exit(3);
	}
	
	//if arg2 is a dir
	if(S_ISDIR(stat2.st_mode)) {
		//1. check these two directory is not the same
		getFullPath(argv[1],argv[2],temp);
		if(stat(temp,&stat_temp) != -1) {
			if(stat1.st_ino == stat_temp.st_ino &&
					 stat1.st_dev == stat_temp.st_dev) {
				fprintf(stderr, "%s: '%s' and '%s' are the same file", 
					argv[0], argv[1],temp);
				exit(5);
			}
		}	
		//2.copy
		copyDir(argv[1], argv[2]);
	}
	//if arg2 is a file
	else if(stat1.st_mode == stat2.st_mode) {
		//1.check these two file is not the same
		if(stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev) {
			fprintf(stderr, "%s: '%s' and '%s' are the same file", 
					argv[0], argv[1],argv[2]);
			exit(5);
		}
		//2.copy
		copyFile(argv[1], argv[2]);
	}
	else {
		fprintf(stderr, "Cannot copy files with differnt mode\n");
		exit(4);
	}			
	

	//Everything goes well
	return(0);
}

void copyDir(char *file, char *dir) {


}

void copyFile(char *file1, char *file2)	{
	
}

void  getFullPath(char *file, char *dir, char *dirf) {
	char *path1 = realpath(file,NULL);
	char *fileName;
	char *temp;
	int idx = 0;

	fileName = strrchr(path1, '/');
//	printf("2:  %s\n",dirf);
	temp = &dir[0];
	idx = 0;
	while(*temp != '\0')
		dirf[idx++] = *temp++;
	dirf[idx] = '\0';
//	printf("First %s\n",dirf);
	strcat(dirf,fileName);
//	printf("dirf %s\n",dirf);
}












