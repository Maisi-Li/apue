#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "sws.h"

#define MAX_CONNECTION 1000;

int dFlag;
int hFlag;
int cFlag;
int iFlag;
int lFlag;
int pFlag;

char *server_addr;
char *server_port;


void usage();
void initialization();	
int main( int argc, char* argv[]) {
	int c;
	while((c = getopt(argc, argv, "dhc:i:l:p:")) != -1) {
		switch(c) {
			case 'd':
				dFlag = 1;
				lFlag = 0;
				break;
			case 'h':
				hFlag = 1;
				usage();
				break;
			case 'c':
				cFlag = 1;
				// to do
				break;
			case 'i':
				iFlag = 1;
				server_addr = optarg;
				break;
			case 'l':
				lFlag = 1;
				dFlag = 0;
				// to do
				break;
			case 'p':
				pFlag = 1;
				server_port = optarg;
		}
	}
	
	argc -= optind;
	argv += optind;
	
	startServer();
	exit(0);

}

void usage() {
	fprintf(stderr, "sws [-dh] [-c dir][-i address][-l file][-p port] dir\n");
	fprintf(stderr, "-c dir Allow execution of CGIs from the given directory.\n");
	fprintf(stderr, "-d Enter debugging mode.\n");
	fprintf(stderr, "-h Print a short usage summary and exit.\n");
	fprintf(stderr, "-i address Bind to the given IPv4 or IPv6 address. If not provided, sws will listen on all IPv4 andIPv6 addresses on this host.\n");
	fprintf(stderr, "-l file Log all requests to the givenfile.\n");
	fprintf(stderr, "-p port Listen on the givenport. If not provided,sws will listen on port 8080.\n");
	exit(1);
}

void initialization() {
	server_addr = NULL;
	server_port = "8080";	
}

