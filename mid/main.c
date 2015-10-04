#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdio.h>

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
	
}
int	aflg, Aflg, Cflg, cflg, dflg, Fflg, fflg, hflg, iflg, kflg, lflg, nflg;
int 	qflg, Rflg, rflg, Sflg, sflg, tflg, uflg, uflg, wflg, xflg, oneflg;
char	*curPath = ".";
int main() {
	
	if(--argc > 0 && *argv[1] == '-') {
		argv++;
		while(*++*argv) switch (**argv)	{
		
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
		
	if(fflg) {
		aflg++;
		lflg = 0;
		sflg = 0;
		tflg = 0;
		
	}

	if(argc == 0) {
		argc++;
		argv = &curPath - 1;
	}

	
	}	
return(0);
}





















