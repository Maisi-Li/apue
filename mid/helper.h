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
#include "ls.h"

int compare(const FTSENT **, const FTSENT **);
Length getLength(const FTSENT *pChild);

void resetBlock(char*, long);
void resetSize(char*, long);
double humanizeNumber(double, char*);
#endif
