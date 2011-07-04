#ifndef __EPOLL_HPP__
#define __EPOLL_HPP__

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/epoll.h>	/* basic epoll */
#include	<fcntl.h>		/* for nonblocking */
#include	<errno.h>
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */

#include	<pthread.h>


//==================================
#include	<unistd.h>
#include	<sys/wait.h>
//==================================

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include <boost/noncopyable.hpp>

#include "sock.hpp"

typedef struct sockaddr SA;




typedef void (epollFun) (const int );

const int MAX_EVENTS = 3;


class myEpoll : private boost::noncopyable {
private:
	mySock sock;
	int epollfd;
	int listen_sock, conn_sock;
	int nfds;
	struct epoll_event ev, events[MAX_EVENTS];
	epollFun *handler;
	// system call:
	int Epoll_create(int size);
	int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	int Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
	
	void setnonblocking(int fd);

	
public:
	myEpoll();
	~myEpoll();
	void start();
	void loop();
	void setHandler( epollFun *func );
	
	
};

#endif
