#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <sys/types.h>
#include <fts.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ls.h"
#include "helper.h"

#define	MAXFILES 1024


void traverse(int argc, char* argv[], int options);

int flg_A = 0;
int flg_a = 0;

// sorting flag
SortingMethod flg_sort = sortByName;
int flg_noSort = 0; 
int flg_R = 0;

// Display flag, default output would be terminal 
DisplayFormat flg_display = in_C;
NonPrintableMethod flg_nonPrintable = in_q;

// 
char* pWidth = NULL;
int main(int argc,char *argv[]) {

	char* currentPath[] = {".",NULL};
        int opt;
	int fts_options = FTS_PHYSICAL;

	setprogname((char*)argv[0]);

	// super user
	if(geteuid() == 0)
		flg_A = 1;
	// if arguments is NOT a terminal 
	if(!isatty(STDOUT_FILENO)) {
		flg_display = in_1;
		flg_nonPrintable = in_w;
}
	
        while((opt = getopt(argc, argv, "1ABCFLRSTWabcdfghiklmnopqrstuwx")) != -1) {
                switch(opt) {
		case '1': flg_display = in_1; break;
		case 'a': flg_a = 1; break;
		case 'A': flg_A = 1; break;
		case 'c': flg_sort = sortByCTime; break;
		case 'C': flg_display = in_C; break;
		case 'f': flg_noSort = 1; break;		
//		case 'h': flg_nonPrintable = 
		case 'l': flg_display = in_l; break;
		case 'n': flg_display = in_n; break;
		case 'q': flg_nonPrintable = in_q; break;
		case 'R': flg_R = 1; break;
                case 'S': flg_sort = sortBySize; break;
		case 't': flg_sort = sortByMTime; break;
		case 'u': flg_sort = sortByATime; break;
		case 'w': flg_nonPrintable = in_w; break;
		case 'x': flg_display = in_x; break;
                default: break;
                }
        }

	argc -= optind;
	argv += optind;
	
	// if no operand is given, then list current directory path
	if(argc == 0) {
		argv = currentPath;
		argc++;
	}

	//check if . or .. would be show
	if(!flg_A && flg_a)
		fts_options |= FTS_SEEDOT;
	
	
	traverse(argc, argv, fts_options);
	exit(0);
}

void traverse(int argc, char* argv[], int options) {

	FTS *pFTS = NULL;
	FTSENT *pFTSRead = NULL;
	FTSENT *PFTSChildren = NULL;
	
	if((pFTS = fts_open(argv, options, flg_noSort? NULL : compare)) == NULL) {
		fprintf(stderr,"%s: %s\n",getprogname(), strerror(errno));
	}

}














