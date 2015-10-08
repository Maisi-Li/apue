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
#include "helper.h"

#define	MAXFILES 1024


int flg_A = 0;
int flg_a = 0;

// sorting flag
SortingMethod flg_sort = sortByName;
int flg_noSort = 0; 
int flg_R = 0;

int main(int argc,char *argv[]) {

	FTS *pFtsRead = NULL;
        FTSENT *pFtsChildren = NULL;
        FTSENT *pFtsParent = NULL;
        int opt;
	int fts_options = 0;

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
	
	fts_options = FTS_PHYSICAL;
        while((opt = getopt(argc, argv, "1ABCFLRSTWabcdfghiklmnopqrstuwx")) != -1) {
                switch(opt) {
		case 'c': flg_sort = sortByCTime; break;
		case 'f': flg_noSort = 1; break;		
		
		case 'R': flg_R = 1; break;
                case 'S': flg_sort = sortBySize; break;
		case 't': flg_sort = sortByMTime; break;
		case 'u': flg_sort = sortByATime; break;
		
                default: break;
                }
        }

	argc -= optind;
	argc += optind;

	if((pFtsRead = fts_open(argv, fts_options,compare)) == NULL) {
		fprintf(stderr, "read error!\n");
		exit(1);	
	}
	while( (pFtsParent = fts_read(pFtsRead)) != NULL)
        {
            pFtsChildren = fts_children(pFtsRead,0);

            if (errno != 0)
            {
                perror("fts_children");
            }

            while ((NULL != pFtsChildren)
                && (NULL != pFtsChildren->fts_link))
            {
                pFtsChildren = pFtsChildren->fts_link;
                printf("%s%s\t", pFtsChildren->fts_path,pFtsChildren->fts_name);
            }
        }	
	fts_close(pFtsRead);
	
	exit(0);
}
















