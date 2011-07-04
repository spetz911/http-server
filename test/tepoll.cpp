#include "../epoll.hpp"
#include <iostream>

const int MAXLINE = 4096;

void
use_fd( int sockfd)
{
	char recvline[MAXLINE];
	char *msg;
	
	int len;
	while( (len = read(sockfd, &recvline, MAXLINE )) >0){
		msg = new char[len+1];
		strcpy(msg, recvline);
		msg[len] = '\0';
		
		std::cout << "sock:" << msg << ";" << std::endl;
		write(sockfd, msg, strlen(msg) );
		delete[] msg;
	}
}

int
main(int argc, char *argv[])
{
	myEpoll epoll;
	epoll.setHandler( &use_fd);
	epoll.start();
	epoll.loop();
	return 0;
}
