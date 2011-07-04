#include "sock.hpp"

mySock::mySock()
{
//	clilen = sizeof(cliaddr);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	listen_sock = Socket(AF_INET, SOCK_STREAM, 0);
}
mySock::mySock(int port)
{
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);
	listen_sock = Socket(AF_INET, SOCK_STREAM, 0);
}
mySock::~mySock()
{
	Close(listen_sock);
}

int
mySock::Close(int sockfd)
{
	int err = close(sockfd);
	if (err < 0) {
		perror("close socket");
		exit(EXIT_FAILURE);
	}
	return err;
}



int
mySock::getListen()
{
	return listen_sock;
}

int
mySock::Accept(int listen_sock)
{
	return Accept(listen_sock, (SA *) &cliaddr, &clilen);
}
int
mySock::Accept(int sock, SA *addr, socklen_t *addrlen)
{
	int conn_sock;
	conn_sock = accept( sock, addr, addrlen);
	if (conn_sock == -1) {
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	return conn_sock;
}

int
mySock::Socket(int family, int type, int protocol)
{
	int sockfd = socket(family, type, protocol);
	if ( sockfd < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	return sockfd;
}

void
mySock::Listen(int listen_sock)
{
	Listen(listen_sock, LISTENQ);
}
void
mySock::Listen(int fd, int backlog)
{
	char	*ptr;
	/* 4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

void
mySock::Bind(int listen_sock)
{
	Bind(listen_sock, (SA *) &servaddr, sizeof(servaddr));
}
void
mySock::Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}
}

void
mySock::Connect(int listen_sock)
{
	Connect(listen_sock, (SA *) &servaddr, sizeof(servaddr));
}

void
mySock::Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}
}

const char *
mySock::Inet_ntop(char *strptr, size_t len)
{
	return Inet_ntop(AF_INET, &servaddr.sin_addr, strptr, len);
}
const char *
mySock::Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const char	*ptr;
	if (strptr == NULL)	{	/* check for old code */
		perror("NULL 3rd argument to inet_ntop");
		exit(EXIT_FAILURE);
	}
	if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL) {
		perror("inet_ntop error");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

void
mySock::Inet_pton(const char *strptr)
{
	Inet_pton(AF_INET, strptr, &servaddr.sin_addr);
}
void
mySock::Inet_pton(int family, const char *strptr, void *addrptr)
{
	int	err = inet_pton(family, strptr, addrptr);
	if (err <= 0) {
		perror("inet_pton error");
		exit(EXIT_FAILURE);
	}
}

char *
mySock::sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char		portstr[7];
    static char str[128];		/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin_port));
			strcat(str, portstr);
		}
		return(str);
	}
	default:
		snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
				 sa->sa_family, salen);
		return(str);
	}
    return (NULL);
}

char *
mySock::Sock_ntop()
{
	return Sock_ntop( (SA *) &cliaddr, clilen );
}
char *
mySock::Sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	char	*ptr;

	if ( (ptr = sock_ntop(sa, salen)) == NULL)
		perror("sock_ntop error");	/* inet_ntop() sets errno */
	return(ptr);
}

void
mySock::setnonblocking(int fd)
{
	int flags;
	flags = fcntl( fd, F_GETFL, 0);
	if ( flags < 0)
		perror("F_GETFL error");
//	flags |= O_NONBLOCK;
	if ( fcntl( fd, F_SETFL, flags|O_NONBLOCK)<0)
		perror("F_SETFL error");
}
