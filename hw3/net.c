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
	int recvfd;
	struct addrinfo hints, *serverinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int temp;
	char pIP[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((temp = getaddrinfo(server_addr, server_port, &hints,&serverinfo)) != 0) {
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(temp));
		exit(1);
	}
	int i = 0;
	for(p = serverinfo; p != NULL; p = p->ai_next, i++) {
		if((sockfd[i] = socket(p->ai_family, 
				p->ai_socktype, 0)) == -1) {
			fprintf(stderr, "socket: %s\n", strerror(errno));
			continue;
		}
		//fprintf(stdout, "ip:%s\n",p->ai_addr->sa_data);
		temp = 1;
		if(setsockopt(sockfd[i], SOL_SOCKET, SO_REUSEADDR,
				 &temp, sizeof(temp)) == -1) {
			fprintf(stderr,"setsockopt: %s\n", strerror(errno));
			exit(1);
          	}	

		if(bind(sockfd[i],p->ai_addr, p->ai_addrlen) == 0) {
		//		inet_ntop(p->ai_family, get_in_addr(p->ai_addr),
		//		 pIP, sizeof(pIP));
		//	fprintf(stderr,"bind ip: %s port: %s\n", pIP,server_port );
			continue;
		} 
	}
		if(listen(sockfd[1],100) == -1) {
			fprintf(stderr, "listen: %s\n", strerror(errno));
			exit(1);
		}
	


	freeaddrinfo(serverinfo);

	while(1) {
		sin_size = sizeof(their_addr);
		recvfd = accept(sockfd[1], (struct sockaddr *) &their_addr, &sin_size);
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
		printf("got connection from %s\n",pIP);
	}
}
