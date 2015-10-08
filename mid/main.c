#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fts.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ls.h"


#define	MAXFILES 1024

/*Functions Definition*/
extern int compare(const FTSENT **, const FTSENT **);

//int	aflg, Aflg, Cflg, cflg, dflg, Fflg, fflg, hflg, iflg, kflg, lflg, nflg;
//int 	qflg, Rflg, rflg, Sflg, sflg, tflg, uflg, wflg, xflg, oneflg;
int flg_A = 0;
int flg_a = 0;

int main(int argc,char *argv[]) {

	FTS *pFtsRead = NULL;
        FTSENT *pFtsChildren = NULL;
        FTSENT *pFtsParent = NULL;
        int opt;
	


	// super user
	if(geteuid() == 0)
		flg_A = 1;
	//Check if argument s a terminal 
	if(isatty(STDOUT_FILENO)) {
		printf("terminal\n");	

	}
	else {
		printf("file\n");
	}
	
        while((opt = getopt(argc, argv, "1ABCFLRSTWabcdfghiklmnopqrstuwx")) != -1) {
                switch(opt) {
		case 'c': flg_sort = sortByCTime; break;
		case 'f': flg_sort = noSort; break;		
		
		case 'R': flg_sort = reverseSort; break;
                case 'S': flg_sort = sortBySize; break;
		case 't': flg_sort = sortByMTime; break;
		case 'u': flg_sort = sortByATime; break;
		
                default: break;
                }
        }
	
	pFtsRead = fts_open(argv + 1,FTS_COMFOLLOW | FTS_NOCHDIR,&compare);

	
	
}
















