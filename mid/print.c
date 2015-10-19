#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pwd.h>
#include<grp.h>
#include<bsd/string.h>
#include<time.h>
#include<math.h>
#include "ls.h"
#include "helper.h"
#include "print.h"

void display_mul(char** pCache, Length len) {
	if(flg_display != in_C && flg_display != in_x)
		return ;
	int i = 0;
	int j = 0;
	int row = 0;
	int num = windowWidth/(len.column + 1);
	if(num == 0)
		num = 1;
	if(flg_display == in_x) {
		for(i = 0; i < pCacheCount; i++) {
			(void) printf("%s ", pCache[i]);
			if((i + 1)%num == 0) 
				(void) printf("\n");
		}
		printf("\n");
	}

	if(flg_display == in_C) {
		row = (int)ceil(pCacheCount*1.0 / num);
		for(i = 0; i < row; i++) {
			for(j = i; j < pCacheCount; j+= row) {
				(void) printf("%s ", pCache[j]);
				
			}
			(void) printf("\n");
		}
}
	pCacheCount = 0;
}
void display_one(FTSENT *pChild, Length len) {
	struct stat *pStat = {0};
	char* pBuf = NULL;
	char pMode[12];
	struct passwd *pUid = NULL;
	struct group *pGid = NULL;
	time_t pTime;

	bzero(pMode, 12);
	pStat = pChild->fts_statp;
	// check a
	if(!flg_dot && pChild->fts_name[0] == '.')
		return;	

	// check inode
	if(flg_i)
		(void)printf("%*lu ",len.l_ino,pStat->st_ino);
	//block
	if(flg_s) {
		pBuf = resetBlock(pStat->st_blocks);
		(void) printf("%*s ", len.l_blocks, pBuf);
		free(pBuf);
	}	
	// check long format
	if(flg_display == in_l || flg_display == in_n) {
		strmode(pStat->st_mode, pMode);
		// mode
		(void) printf("%-*s ", 11, pMode);
		
		// nlink
		(void) printf("%*zu ", len.l_nlink, pStat->st_nlink);
		//uid
		pUid = getpwuid(pStat->st_uid);
		if(flg_display == in_n || pUid == NULL) 
			(void) printf("%*d ", len.l_uid, pStat->st_uid);
		else
			(void) printf("%-*s ", len.l_uid, pUid->pw_name);
			
		//group
		pGid = getgrgid(pStat->st_gid);
		if(flg_display == in_n || pGid == NULL) 
			(void) printf("%*d ", len.l_gid, pStat->st_gid);
		else
			(void) printf("%-*s ", len.l_gid, pGid->gr_name);
		//size
		if(pMode[0] == 'c' || pMode[0] == 'b') {

			(void)	printf("%*d, %*d ", 
				len.l_major, major(pStat->st_rdev),
				len.l_minor, minor(pStat->st_rdev));
		}
		else if(flg_h) {
			pBuf = resetSize(pStat->st_size);	
			(void)printf("%*s ",len.l_size,  pBuf);
			free(pBuf);
		}
		else
			(void)printf("%*zu ",len.l_size, pStat->st_size);
		//time
		pTime = pStat->st_mtime;
		if(flg_sort == sortByCTime)
			pTime = pStat->st_ctime;
		if(flg_sort == sortByATime)
			pTime = pStat->st_atime;
		pBuf = displayTime(pTime);
		(void) printf("%s ", pBuf);
		free(pBuf);	
	}

	//name
	pBuf = displayName(pChild->fts_accpath);
	(void)printf("%s", pBuf);
	free(pBuf);
	// -F
	if(flg_F) 
		(void)printf("%c ", displayChar(pMode));
	(void) printf(" ");
	//symbolic link 
	if((flg_display == in_l || flg_display == in_n) && pMode[0] == 'l') {
		pBuf = displayLink(pChild);
		(void) printf("%s", pBuf);
		free(pBuf);
	}

	(void)printf("\n");
}
