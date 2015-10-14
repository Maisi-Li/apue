#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pwd.h>
#include<grp.h>
#include<bsd/string.h>
#include<time.h>
#include "ls.h"
#include "helper.h"
#include "print.h"


void display_one(FTSENT *pChild, Length len) {
	struct stat *pStat = {0};
	char pBuf[1024];
	char pMode[12];
	struct passwd *pUid = NULL;
	struct group *pGid = NULL;
	time_t pTime;

	bzero(pBuf, 1024);
	pStat = pChild->fts_statp;
	// check a
	

	// check inode
	if(flg_i)
		(void)printf("%*lu ",len.l_ino,pStat->st_ino);
	//block
	if(flg_s) {
		resetBlock(pBuf, pStat->st_blocks);
		(void) printf("%*s ", len.l_blocks, pBuf);
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
			resetSize(pBuf, pStat->st_size);	
			(void)printf("%*s ",len.l_size,  pBuf);
		}
		else
			(void)printf("%*zu ",len.l_size, pStat->st_size);
		//time
		pTime = pStat->st_mtime;
		if(flg_sort == sortByCTime)
			pTime = pStat->st_ctime;
		if(flg_sort == sortByATime)
			pTime = pStat->st_atime;
		displayTime(pTime);	
	}

	//name
	(void)printf("%s", pChild->fts_accpath);
	// check -F

	(void)printf("\n");
}
