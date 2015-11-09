#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<err.h>
#include<errno.h>
#include "sws.h"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void startServer() {
	int sockfd[2] = {0,0};
	int sockCount = 0;
	int recvfd;
	struct addrinfo hints, *serverinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int temp;
	int i;
	fd_set readSockSet, copySet; 
	int sockMax = 0;
	char pIP[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	FD_ZERO(&readSockSet);
	FD_ZERO(&copySet);
	if((temp = getaddrinfo(server_addr, server_port, &hints,&serverinfo)) != 0) {
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(temp));
		exit(1);
	}

	for(p = serverinfo; p != NULL; p = p->ai_next, sockCount++) {
		if((sockfd[sockCount] = socket(p->ai_family, 
				p->ai_socktype, 0)) == -1) {
			fprintf(stderr, "socket: %s\n", strerror(errno));
			continue;
		}
		
		int yes = 1;
		if(p->ai_family == AF_INET)
			temp = setsockopt(sockfd[sockCount], SOL_SOCKET, SO_REUSEADDR, 
				&yes, sizeof(yes));
		else
			temp = setsockopt(sockfd[sockCount], IPPROTO_IPV6, IPV6_V6ONLY,
				&yes, sizeof(yes));
		if(temp == -1) {
			fprintf(stderr, "setsockopt: %s\n", strerror(errno));
			continue;
		}

		if(bind(sockfd[sockCount],p->ai_addr, p->ai_addrlen) == -1) {
			fprintf(stderr, "bind: %s\n", strerror(errno));
			continue;
		} 
	
		if(listen(sockfd[sockCount],100) == -1) {
			fprintf(stderr, "listen: %s\n", strerror(errno));
			continue;
		}
		else {

		}

		FD_SET(sockfd[sockCount], &readSockSet);
		if(sockfd[sockCount] > sockMax)
			sockMax = sockfd[sockCount];	
	}
	printf("start listen ....\n");
	freeaddrinfo(serverinfo);


	while(1) {
		copySet = readSockSet;
		if(select(sockMax + 1, &copySet, NULL, NULL, NULL) == -1) {
			fprintf(stderr, "select: %s\n", strerror(errno));
			exit(1);
		}
		for(i = 0;i < sockCount; i++) {
			if(FD_ISSET(sockfd[i], &copySet)) {
				sin_size = sizeof(their_addr);
				(void) memset(&their_addr, 0, sin_size);
				recvfd = accept(sockfd[i], (struct sockaddr *) &their_addr,
						 &sin_size);
				if(recvfd == -1) {
					fprintf(stderr, "accept error: %s\n", 
						strerror(errno));
					break;
				}
			if(inet_ntop(their_addr.ss_family, 
					get_in_addr((struct sockaddr *)&their_addr), 
					pIP, sizeof(pIP)) == NULL) {
				fprintf(stderr, "inet_ntop%s\n", strerror(errno));
				continue;
			}	

				printf( "get connection from %s \n", pIP);
			}
		}

	}
/*

	while(1) {
		sin_size = sizeof(their_addr);
		recvfd = accept(sockfd[0], (struct sockaddr *) &their_addr, &sin_size);
		if(recvfd == -1) {
			fprintf(stderr, "accept error: %s\n", strerror(errno));
			break;
		}		
		
		if(inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *)&their_addr),
				pIP, sizeof pIP) == NULL) {
			fprintf(stderr, "inet_ntop\n");
			continue;	
		}
	//	printf("got connection from %s\n",pIP);
		int pid;
		if((pid = fork()) < 0) {
			fprintf(stderr, "fork:%s\n", strerror(errno));
			exit(1);
		}
		else if (pid == 0) {
			fprintf(stdout, "get connection from %s\n", pIP);
			close(recvfd);
		}
		close(recvfd);
	}
*/
}
