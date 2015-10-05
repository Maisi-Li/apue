#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>
#include <stdlib.h>
#define	MAXFILES	1024

struct fbuf {
	char *f_name;
	char f_type;
	short f_ino;
	short f_flags;
	short f_nlink;
	short f_uid;
	short f_gid;
	long  f_size;
	long  f_time;
	
};
int	aflg, Aflg, Cflg, cflg, dflg, Fflg, fflg, hflg, iflg, kflg, lflg, nflg;
int 	qflg, Rflg, rflg, Sflg, sflg, tflg, uflg, wflg, xflg, oneflg;
char	*curPath = ".";
int main(int argc,char *argv[]) {
	
		
		case 'a':
			aflg++;
			continue;
	
		case 'A':
			Aflg++;
			continue;	

		case 'l':
			lflg++;
			continue;
		
		default:
			continue;
		}
	}	

	if(argc == 0) {
		argc++;
		argv = &curPath - 1;
	}
	
	
	
		
return(0);
}





















