#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

void doprocessing(int sock);

int main(int argc, char *argv[]){
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n, pid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd<0){
		perror("ERROR OPENING SOCKET");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR ON BINDING");
		exit(1);
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if(newsockfd < 0){
			perror("ERROR ON ACCEPT");
			exit(1);
		}

		pid = fork();
		
		if(pid < 0){
			perror("ERROR ON FORK");
			exit(1);
		}

		if(pid == 0){
			close(sockfd);
			doprocessing(newsockfd);
			exit(0);
		}

		else{
			close(newsockfd);
		}
	}
	return 0;
}	

void doprocessing(int sock){
	int n;
	char buffer[256];
	bzero(buffer, 256);
	n = read(sock,buffer,255);

	if(n < 0){
		perror("ERROR READING FROM SOCKET");
		exit(1);
	}

	printf("Here is the message %s\n",buffer);

	n = write(sock, "I got your message", 18);

	if (n < 0){
		perror("ERROR WRITING TO SOCKET");
		exit(1);
	}
}