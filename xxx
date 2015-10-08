#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fts.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

int flg_S;
int compare (const FTSENT**, const FTSENT**);

int main(int argc, char* const argv[]) {
	
	FTS *pFtsRead = NULL;
	FTSENT *pFtsChild = NULL;
	FTSENT *pFtsParent = NULL;
	int opt;

	while((opt = getopt(argc, argv, "1ABCFLRSTWabcdfghiklmnopqrstuwx")) != -1) {
		switch(opt) {
		case 'S': flg_S = 1;break;	
		default: break;
		}		
	} 
	
	pFtsRead = fts_open(argv + 1,FTS_COMFOLLOW | FTS_NOCHDIR,&compare);

    if (pFtsRead != NULL)
    {
        while( (pFtsParent = fts_read(pFtsRead)) != NULL)
        {
            pFtsChild = fts_children(pFtsRead,0);

            if (errno != 0)
            {
                perror("fts_children");
            }

            while ((NULL != pFtsChild)
                && (NULL != pFtsChild->fts_link))
            {
                pFtsChild = pFtsChild->fts_link;
                printf("%-14.8s", pFtsChild->fts_name);
            }
        }
        fts_close(pFtsRead);
    }
    printf("\n");
    return 0;
}

int compare(const FTSENT** one, const FTSENT** two)
{
	if(flg_S) {
		if((*one)->fts_statp->st_size > (*two)->fts_statp->st_size)
			return (-1);
		if((*one)->fts_statp->st_size < (*two)->fts_statp->st_size)
			return (1);
		return (strcoll((*one)->fts_name,(*ttwoo)->fts_name));
	}
    return (strcmp((*one)->fts_name, (*two)->fts_name));
}
