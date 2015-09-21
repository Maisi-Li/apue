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
void copyFile(char *command, char *file1, char *file2);
void copyDir(char *command,char *file, struct stat *fstat, char *dir);
char* getFullPath(char *file, char *dir, char *temp);


int main(int argc, char* argv[]) {
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
		if(stat(getFullPath(argv[1],argv[2],temp),&stat_temp) != -1) {
		//	printf("argv1: %s   temp: %s\n",argv[1],temp);
			if(stat1.st_ino == stat_temp.st_ino &&
					 stat1.st_dev == stat_temp.st_dev) {
				fprintf(stderr, "%s: '%s' and '%s' are the same file", 
					argv[0], argv[1],temp);
				exit(5);
			}
			copyFile(argv[0],argv[1],temp);
		}
		else	
			copyDir(argv[0],argv[1], &stat1,temp);
	}
	//if arg2 is a file
	else  {
		//1.check these two file is not the same
		if(stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev) {
			fprintf(stderr, "%s: '%s' and '%s' are the same file", 
					argv[0], argv[1],argv[2]);
			exit(5);
		}
		//2.copy
		copyFile(argv[0],argv[1],argv[2]);
	}
			
	

	//Everything goes well
	return(0);
}

void copyDir(char *command, char *file, struct stat *fstat, char *dir) {
	int inputFd, outputFd;	
	size_t readIn, writeOut;
	char buffer[BUFFER_SIZE];
	//printf("test temp: %s\n",dir);
	if((inputFd = open(file,O_RDONLY)) == -1) {
		fprintf(stderr,"%s: %s: %s\n",
				command, file, strerror(errno));
		exit(2);
	}
	if((outputFd = open(dir, O_WRONLY | O_CREAT|O_TRUNC,S_IWUSR)) == -1) {
	       	fprintf(stderr, "%s: %s: %s\n", 
				command, dir, strerror(errno));
		exit(4);
	}
//		fchmod(outputFd, fstat->st_mode);
//		fchmod(outputFd, t);
	while((readIn = read(inputFd, &buffer,BUFFER_SIZE)) > 0) {
		writeOut = write(outputFd, &buffer, (size_t)readIn);
		if(writeOut != readIn) {
			fprintf(stderr,"%s: write error", "tcp");
			exit(3);
		}
//		printf("buffer:%s\n",buffer);
	}
	
	
	fchmod(outputFd, fstat->st_mode);
	close(inputFd);
	close(outputFd);
	
}

void copyFile(char *command, char *file1, char *file2)	{
	int inputFd, outputFd;	
	ssize_t readIn, writeOut;
	char buffer[BUFFER_SIZE];

	if((inputFd = open(file1,O_RDONLY)) == -1) {
		fprintf(stderr,"%s: %s: %s\n",
				command, file1, strerror(errno));
		exit(2);
	}
	if((outputFd = open(file2,O_WRONLY | O_TRUNC) ) == -1) {
		fprintf(stderr, "%s: %s: %s",
				 command, file2,strerror(errno));
		exit(2);
		}
	
	while((readIn = read(inputFd, &buffer, BUFFER_SIZE)) > 0) {
		writeOut = write(outputFd, &buffer, (ssize_t)readIn);
		if(writeOut != readIn) {
			fprintf(stderr,"%s: write error", command);
			exit(6);
		}
	}
	close(inputFd);
//	fchmod(outputFd,0744);
	close(outputFd);
}

char* getFullPath(char *file, char *dir, char *dirf) {
	char *path1 = realpath(file,NULL);
	char *fileName;
	char *path2 = realpath(dir,NULL);
	char *temp;
	char *value = (char*) malloc(4096*sizeof(char));
	int idx = 0;

	fileName = strrchr(path1, '/');
	fileName++;
	//real full path
	temp = &path2[0];
	idx = 0;
	while(*temp != '\0')
		value[idx++] = *temp++;
	value[idx++] = '/';
	value[idx] = '\0';
	strcat(value,fileName);
//	printf("value:%s\n",value);
	//relative path
	temp = &dir[0];
	idx = 0;
	while(*temp != '\0')
		dirf[idx++] = *temp++;
	if(dirf[idx - 2] != '/')
		dirf[idx++] = '/';
	dirf[idx] = '\0';
//	printf("First %s\n",dirf);
	strcat(dirf,fileName);
//	printf("dirf in getFullPath:  %s\n",dirf);
	return value;
}
