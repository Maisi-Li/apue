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
	int sockfd, recvfd;
	struct addrinfo hints, *serverinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int temp;
	char pIP[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((temp = getaddrinfo(server_addr, server_port, &hints,&serverinfo)) != 0) {
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(temp));
		exit(1);
	}
	temp = 0;

	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}
		//fprintf(stdout, "ip:%s\n",p->ai_addr->sa_data);
		if(bind(sockfd,p->ai_addr, p->ai_addrlen == 0)) {
			break;
		} 
	}

	freeaddrinfo(serverinfo);

	if(p == NULL) {
		fprintf(stderr,"socket or bind");
		exit(1);
	} 

	if(listen(sockfd,100) == -1) {
		fprintf(stderr, "listen: %s\n", strerror(errno));
		exit(1);
	}	
	
	while(1) {
		sin_size = sizeof(their_addr);
		recvfd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if(recvfd == -1) {
			fprintf(stderr, "accept error");
			continue;
		}		

		inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *)&their_addr),
				pIP, sizeof pIP);
		printf("got connection from %s\n",pIP);
	}
}
