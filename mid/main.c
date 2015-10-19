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
#include<err.h>
#include "ls.h"
#include "helper.h"
#include "print.h"
#define	MAXFILES 1024


void traverse(int argc, char* argv[], int options);

int flg_A = 0;
int flg_a = 0;

// sorting flag
SortingMethod flg_sort = sortByName;
int flg_noSort = 0;
int flg_reverse = 0; 
int flg_R = 0;

// Display flag, default output would be terminal 
DisplayFormat flg_display = in_C;
NonPrintableMethod flg_nonPrintable = in_q;

int flg_h = 0;
int flg_s = 0;
Block flg_block = in_k;

int flg_i = 0;
int flg_F = 0;
// 
char* pWidth = NULL;
Length len = {0};

int flg_dot = 0;
int flg_d = 0;
int flg_printBefore = 0;
char* pBuf;

char** pCache = NULL;
int pCacheCount = 0;
int main(int argc,char *argv[]) {

	char* currentPath[] = {".",NULL};
        int opt;
	int fts_options = FTS_PHYSICAL;
	pBuf = calloc(1024, sizeof(char));
	setprogname((char*)argv[0]);
	// super user
	if(geteuid() == 0)
		flg_A = 1;
	// if arguments is NOT a terminal 
	if(!isatty(STDOUT_FILENO)) {
		flg_display = in_1;
		flg_nonPrintable = in_w;
	}
	else {
		flg_display = in_C;
		flg_nonPrintable = in_q;
	}
		
	
        while((opt = getopt(argc, argv, "1ACFLRSTWacdfghiklmnopqrstuwx")) != -1) {
                switch(opt) {
		case '1': flg_display = in_1; break;
		case 'a': flg_a = 1; break;
		case 'A': flg_A = 1; break;
		case 'c': flg_sort = sortByCTime; break;
		case 'C': flg_display = in_C; break;
		case 'd': flg_d = 1; break;
		case 'f': flg_noSort = 1; break;		
		case 'F': flg_F = 1; break;
		case 'h': flg_h = 1; flg_block = in_h; break;
		case 'i': flg_i = 1; break;
		case 'k': flg_block = in_k; break; 
		case 'l': flg_display = in_l; break;
		case 'n': flg_display = in_n; break;
		case 'q': flg_nonPrintable = in_q; break;
		case 'r': flg_reverse = 1; break;
		case 'R': flg_R = 1; break;
		case 's': flg_s = 1; break;
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
	if(!flg_A && flg_a) {
		fts_options |= FTS_SEEDOT;
		flg_dot = 1;
	}

	
	traverse(argc, argv, fts_options);
	exit(0);
}

void traverse(int argc, char* argv[], int options) {

	FTS *pFTS = NULL;
	FTSENT *pFTSRead = NULL;
	FTSENT *pFTSChildren = NULL;
	FTSENT *pFTSTemp = NULL;
	if((pFTS = fts_open(argv, options, flg_noSort? NULL : compare)) == NULL ||
			errno != 0) 
		err(1,":fts_open ");

	pFTSChildren = fts_children(pFTS, 0);
	pFTSTemp = pFTSChildren;
	
	if(pFTSChildren == NULL && errno != 0)
		err(1,":traverse :before getLength ");
	
	len = getLength(pFTSChildren);
	//1. print operand as file 
	createBuf(&pCache, len);
	if(flg_d) {
		while(pFTSChildren != NULL) {
			if(!verifyFTS(pFTSChildren)) {
				if(!prepareBuf(pCache, pFTSChildren, len))
				display_one(pFTSChildren, len);
			}	
			pFTSChildren = pFTSChildren->fts_link;
		}	
		display_mul(pCache, len);
		freeBuf(pCache);
		(void)fts_close(pFTS);
		return ;
		
	}

	//only traverse file
	createBuf(&pCache,len);
	pFTSChildren = pFTSTemp;
	while(pFTSChildren != NULL) {
		if(!verifyFTS(pFTSChildren) && 
				!S_ISDIR(pFTSChildren->fts_statp->st_mode)) {
			if(!prepareBuf(pCache, pFTSChildren, len))
			display_one(pFTSChildren, len);
			flg_printBefore = 1;
		}
		pFTSChildren = pFTSChildren->fts_link;
	}
	display_mul(pCache,len);
	if(flg_printBefore)
		printf("\n");
	freeBuf(pCache);
 
	while((pFTSRead = fts_read(pFTS)) != NULL) {
		// traverse directory
		if(verifyFTS(pFTSRead)) 
			continue;
		
		if(pFTSRead->fts_level != 0 && pFTSRead->fts_name[0] == '.'
				&& !flg_dot) {
			fts_set(pFTS, pFTSRead, FTS_SKIP);
			continue;
		}		

		if(pFTSRead->fts_level == 0 && pFTSRead->fts_info == FTS_D) {
		
			pFTSChildren = fts_children(pFTS, 0);
			if(verifyFTS(pFTSChildren))
				continue;
			len = getLength(pFTSChildren);
			createBuf(&pCache, len);
			if(argc > 1 || flg_R || flg_printBefore) 
				( void)printf("%s:\n",
					 pFTSChildren->fts_parent->fts_accpath);
	
			if(flg_s) {
				pBuf = resetBlock(len.total_b);
				(void)printf("total %s\n",pBuf);
			}
			while(pFTSChildren) {
				if(!verifyFTS(pFTSChildren)) {
					if(!prepareBuf(pCache, pFTSChildren, len))
						display_one(pFTSChildren, len);
						flg_printBefore = 1;

				}
				pFTSChildren = pFTSChildren -> fts_link;
			
			}
			display_mul(pCache, len);
			free(pCache);
			printf("\n");
		}
	//	if(pFTSRead->fts_level > 0&& pFTSRead->fts_accpath[0] != '.')	
	//		printf("%s\n ", pFTSRead->fts_accpath);
		//traverse recursively
		if(flg_R && pFTSRead->fts_level > 0 && pFTSRead->fts_info == FTS_D) {
			
			pFTSChildren = fts_children(pFTS, 0);
			if(verifyFTS(pFTSChildren))
				continue;
			len = getLength(pFTSChildren);
			//avoid sub dir begin with '.'
/*			pBuf = strchr(pFTSChildren->fts_path,'/');
			if(pBuf[1] == '.' && !flg_dot) 
				continue;
*/
			(void)printf("%s:\n", pFTSChildren->fts_path);
			
			if(flg_s) {
				pBuf = resetBlock(len.total_b);
				(void)printf("total %s\n",pBuf);
			}
			while(pFTSChildren) {
				if(!verifyFTS(pFTSChildren)) {
					display_one(pFTSChildren, len);
				}
				pFTSChildren = pFTSChildren -> fts_link;
		
			}
		(void)printf("\n");			
		}
	}
	fts_close(pFTS);
	
}	















