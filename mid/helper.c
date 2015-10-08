#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fts.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include "ls.h"
#include "helper.h"

int compareByMethod(struct stat* a, struct stat* b);

int compare (const FTSENT** a, const FTSENT** b) {
	int info_a, info_b;
	int result = 0;
	info_a = (*a)->fts_info;
	info_b = (*b)->fts_info;
	
	if(info_a == FTS_ERR || info_b == FTS_ERR)
		return 0;
	

	if(flg_noSort)
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
				
		
	default: return 0;
	}
}
