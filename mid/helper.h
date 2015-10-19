#ifndef HELPER_H_
#define HELPER_H_

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
#include <stdint.h>
#include <time.h>
#include "ls.h"

int compare(const FTSENT **, const FTSENT **);
Length getLength(const FTSENT *pChild);

char* resetBlock( uint64_t);
char* resetSize(uint64_t);
double humanizeNumber(double, char*);

char* displayTime(time_t time);
char displayChar(char*);
char* displayLink(FTSENT *pChild);
int verifyFTS(FTSENT*);
char* displayName(char* );
#endif
