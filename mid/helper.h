#ifndef HELPER_H_
#define HELPER_H_
#endif

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


int compare(const FTSENT **, const FTSENT **);

