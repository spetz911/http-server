#include	<iostream>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"../sock.hpp"

const int S_PORT = 9877;
const int MAX_EVENTS = 7;

void delay(int n)
{
	while(n-->0);
}

void
reading(int sockfd)
{
	std::string rec;
	char raw[100];
	int len;
	if((len=read(sockfd, &raw, strlen(raw))) >1){
		std::cout << "pipe"<<len<<":";
		raw[len] = '\0';
		std::cout << raw;
		while( (len=read(sockfd, &raw, strlen(raw))) >1){
		raw[len] = '\0';
		std::cout << "<" << len << ">" << raw;
		}
		std::cout << ";" << std::endl;
	}
}

std::string hello ("GET 1.py HTTP/1.1\r\n");
std::string trash ("GET sdsadasd sad asdas HTTP/1.1\r\n dsfds fdsa \r");

void *
str_cli(void* input)
{
	int sockfd = *((int *) input);
	int n;
	std::string str;

	n = 4;
	while(n-->0){
		write(sockfd, trash.c_str(), trash.length() );
		sleep(1);
		std::cout << "+1" << std::endl;
	}
	return NULL;


	
	n = 2;
	while(n-->0){
		write(sockfd, hello.c_str(), hello.length() );
		std::cout << "+1" << std::endl;
	}
	return NULL;
	
		
	while(std::getline( std::cin , str)) {
		write(sockfd, str.data(), str.length() );
	}
	return NULL;
}


int
main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "argv: no <IPaddress>" << std::endl;
		exit(1);
	}

	//initialize
	int epollfd;
	int listen_sock, nfds;

	struct epoll_event ev, events[MAX_EVENTS];

	
	mySock sock(S_PORT);
	listen_sock = sock.getListen();

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	
	epollfd = epoll_create(MAX_EVENTS);

	sock.Inet_pton(argv[1]);
	sock.Connect(listen_sock);
	sock.setnonblocking(listen_sock);
	
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev);
	
	//! create new thread
	pthread_t t;
	pthread_create(&t, NULL, &str_cli, &listen_sock);
	pthread_detach(t);
	
	
	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		int n;
		for (n = 0; n < nfds; ++n) {
			// by analogy with True branch, epoll ok
			printf("Ready\n");
	
			reading(events[n].data.fd);
		}
	}
	exit(0);
}
