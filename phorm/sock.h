#ifndef __SOCK_HPP__
#define __SOCK_HPP__


#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/epoll.h>	/* basic epoll */
#include	<fcntl.h>		/* for nonblocking */
#include	<errno.h>
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */

#include <pthread.h>


//==================================
#include	<unistd.h>
#include	<sys/wait.h>
//==================================

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>


#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<errno.h>

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */

typedef struct sockaddr SA;
#include <boost/noncopyable.hpp>

const int SERV_PORT = 1408;
const int LISTENQ   = 1024;	/* 2nd argument to listen() */




class mySock : private boost::noncopyable {
private:
	int listen_sock, conn_sock;
	struct sockaddr_in cliaddr;
	struct sockaddr_in servaddr;
	socklen_t clilen;

	// system call:
	int Socket(int family, int type, int protocol);
	int Accept(int sock, SA *addr, socklen_t *addrlen);
	void Listen(int fd, int backlog);
	void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
	void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
	int Close(int sockfd);
	
	char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);
	char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
	const char *Inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
	void Inet_pton(int family, const char *strptr, void *addrptr);

	
public:
	mySock();
	mySock(unsigned short port);
	~mySock();
	int getListen();
	
	int Accept(int listen_sock);
	
	void Listen(int listen_sock);
	void Bind(int listen_sock);
	void Connect(int listen_sock);
	
	
	// stuff funcs:
	void Inet_pton(const char *strptr);
	const char *Inet_ntop(char *strptr, size_t len);
	void setnonblocking(int fd);
	char *Sock_ntop();
		
};


#endif
