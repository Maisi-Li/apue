#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fts.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<math.h>
#include<stdint.h>
#include<time.h>
#include "ls.h"
#include "helper.h"

#define SIXMONTHS ()
int compareByMethod(struct stat* a, struct stat* b);
int getIntLength(int);
//
int windowWidth; // default width 

void displayTime(time_t before) {
	time_t now;
	char pBuf[128];
	struct tm* time_info;
	now = time(NULL);
	double gap = abs(now - before);

	if((time_info = localtime(&before)) != NULL) {
		// six month = 60 * 60 *24*365 /2  = 15768000
		if(gap < 15768000) 
			(void) strftime(pBuf, 16, "%b %d %H:%M", time_info);
		else
			(void) strftime(pBuf, 16, "%b %d %Y", time_info);
		printf("%s ", pBuf);		
	}	

}


Length getLength(const FTSENT *pChild) {
	char *pColumns = NULL;
	Length s_length = {0};
	int temp_len = 0;
	char pBuf[1024];
	struct stat *tempStat = {0};
	struct passwd *pUid = NULL;
	struct group *pGid = NULL;
	int hasDevice = 0;

	uint64_t maxSize = 0;
	uint64_t maxBlock = 0;

	pColumns = getenv("COLUMNS");
	if(pColumns != NULL && atoi(pColumns) > 0) 
		windowWidth = atoi(pColumns);

	while(pChild != NULL) {
		s_length.count++;
		
		// get access path (NAME)
		if(strlen(pChild->fts_accpath) > s_length.l_name)
			s_length.l_name = strlen(pChild->fts_accpath);			
		
		if(pChild->fts_info != FTS_NS)	{
			tempStat = pChild->fts_statp;

			// get inode 
			//(void)snfrintf(pBuf, sizeof(pBuf),"%lu",tempStat->st_ino);
			if(tempStat->st_ino > s_length.l_ino)
				s_length.l_ino = tempStat->st_ino;

			// get nlink
			if(tempStat->st_nlink > s_length.l_nlink)
				s_length.l_nlink = tempStat->st_nlink;

			// get uid
			pUid = getpwuid(tempStat->st_uid);
			if(flg_display == in_n || pUid == NULL)	{
				(void)snprintf(pBuf, sizeof(pBuf),"%u",
					tempStat->st_uid);
				temp_len = strlen(pBuf);
			}
			else 
				temp_len = strlen(pUid->pw_name);
			if(temp_len > s_length.l_uid)
				s_length.l_uid = temp_len;
			
			//get gid
			pGid = getgrgid(tempStat->st_gid);			
			if(flg_display == in_n || pUid == NULL)	{
				(void)snprintf(pBuf, sizeof(pBuf),"%u",
					tempStat->st_gid);
				temp_len = strlen(pBuf);
			}
			else 
				temp_len = strlen(pGid->gr_name);
			if(temp_len > s_length.l_gid)
				s_length.l_gid = temp_len;


			// get size
			hasDevice = 0; 
			if((tempStat->st_mode & S_IFMT) == S_IFBLK 
				|| (tempStat->st_mode & S_IFMT) == S_IFCHR) 
				hasDevice = 1;

			if(hasDevice) {
				if(major(tempStat->st_rdev) > s_length.l_major)
					s_length.l_major = major(tempStat->st_rdev);
				
				if(minor(tempStat->st_rdev) > s_length.l_minor)
					s_length.l_minor = minor(tempStat->st_rdev);
			
			}
			else {
				if(tempStat->st_size > maxSize)
					maxSize = tempStat->st_size;
			}

			// get block
			if(tempStat->st_blocks > maxBlock)
				maxBlock = tempStat->st_blocks;

			//
	//		resetSize(pBuf, tempStat->st_size);
	//		printf("%s: %s ", pChild->fts_accpath,
	//				pBuf);
		//	printf("after resize: %0.1f\n", resizeBlock(tempStat->st_blocks));
		}
		//printf
		pChild = pChild->fts_link;
	}// end while

	// count length
	s_length.l_ino = getIntLength(s_length.l_ino);
	s_length.l_nlink = getIntLength(s_length.l_nlink);
	
	//resize size
	//printf("origin size: %zu\n",maxSize);
	if(s_length.l_major != 0 || s_length.l_minor != 0) {
		s_length.l_size = getIntLength(s_length.l_major) +
				 getIntLength(s_length.l_minor) + 2;
	}
	else if(flg_display == in_l || flg_display == in_n) {
		if(flg_h) {
			resetSize(pBuf, maxSize);
			s_length.l_size = strlen(pBuf);
		}
		else {
			s_length.l_size = 
				snprintf(pBuf, sizeof(pBuf), "%zu", maxSize);
		}
	}
		
	//resize block 
	 if (flg_s) {
		resetBlock(pBuf, maxBlock);
		//printf("MaxBlockSize: %s\n", pBuf);		
		s_length.l_blocks = strlen(pBuf);
	}
		
	
	s_length.column = s_length.l_name;
	if(flg_i)
		s_length.column += s_length.l_ino + 1;
	if(flg_s)
		s_length.column += s_length.l_blocks + 1;
	if(flg_F)
		s_length.column += 1;
	
	//printf("finish!\n");

	return s_length;
}

int getIntLength(int a)	{
	int l = 1;
	while(a > 9) {
		l++;
		a/=10;
	}
	return l;
}

void resetSize(char* pBuf, uint64_t size) {
	char unit = ' ';
	double num = 0;
	num = humanizeNumber(size*1.0,&unit);
	if(unit == ' ')
		snprintf(pBuf, sizeof(pBuf), "%d",(int) num);
	else if(num < 10.0) 
		snprintf(pBuf, sizeof(pBuf), "%0.1f%c", num, unit);
	else
		snprintf(pBuf, sizeof(pBuf),"%d%c", (int)num, unit);
}


void resetBlock(char* pBuf, uint64_t block) {
	char* pDefaultSize = NULL;
	long envSize;
	double val;
	char unit = ' ';
	pDefaultSize = getenv("BLOCKSIZE");
	if(pDefaultSize == NULL || atol(pDefaultSize) == 0) {
	// lab system is 1024
		envSize = 1024;
	}
	else
		envSize = atol(pDefaultSize);
	
	//default is -k
	if(flg_block == in_h) {
		val = humanizeNumber(block*512.0, &unit);
		val = val < 0.5f ? 0.5f: floor(val*2)/2;
		(void)snprintf(pBuf, sizeof(pBuf), "%0.1f%c", val, unit);
		
	}
	else {
		val = ceil(block * 512.0/envSize);
		(void)snprintf(pBuf, sizeof(pBuf), "%d", (int)val);
	}
}

double humanizeNumber(double num, char* postfix) {
	char unit[7] = {'\0','K' , 'M', 'G', 'T', 'P', 'E'};
	int index = 0;
	//printf("num = %f\n", num);
	if(num > 1000) {
		num/= 1000;
		index++;
	}
	
	if(postfix != NULL)
		*postfix = unit[index];

	return num;	
}

int compare (const FTSENT** a, const FTSENT** b) {
	int info_a, info_b;
	int result = 0;
	info_a = (*a)->fts_info;
	info_b = (*b)->fts_info;
	
	if(info_a == FTS_ERR || info_b == FTS_ERR) 
		return 0;

	if(info_a == FTS_NS || info_b == FTS_NS) {
		if(info_a != FTS_NS)		
			return (-1);
		else if(info_b != FTS_NS)
			return (1);
		else
			return (strcmp((*a)->fts_name, (*b)->fts_name));
	}
	// direcotry first
	if(S_ISDIR((*a)->fts_statp->st_mode) && !S_ISDIR((*b)->fts_statp->st_mode))
		return 1;

	if(!S_ISDIR((*a)->fts_statp->st_mode) && S_ISDIR((*b)->fts_statp->st_mode))
		return -1;

	if((result = compareByMethod((*a)->fts_statp, (*b)->fts_statp)) == 0)
		result = strcmp((*a)->fts_name, (*b)->fts_name);
	 
	return (flg_R ? -1*result:result);
}

int compareByMethod(struct stat* a, struct stat* b) {
	switch(flg_sort) {
	case sortByATime: 
		if(a->st_atime == b->st_atime)
			return 0;
		return (a->st_atime > b->st_atime ? -1:1);
	case sortByMTime:				
		if(a->st_mtime == b->st_mtime)
			return 0;
		return (a->st_mtime > b->st_mtime ? -1:1);
	case sortByCTime:
		if(a->st_ctime == b->st_ctime)
			return 0;
		return (a->st_ctime > b->st_ctime ? -1:1);
	case sortBySize:
		if(a->st_size == b->st_size)
			return 0;
		return (a->st_size > b->st_size ? -1:1);
		
	default: return 0;
	}

	return 0;
}


