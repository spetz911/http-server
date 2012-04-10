#include <iostream>

#include "epoll.hpp"


/* Miscellaneous constants */
const int MAXLINE = 4096;	/* max text line length */
const int BUFSIZE = 8192;	/* buffer size for reads and writes */

// #include	"my_lib.cpp"

char hello[] = "Hello, People!\r\n";

void
myEpoll::setnonblocking(int fd)
{
	int flags;
	flags = fcntl( fd, F_GETFL, 0);
	if ( flags < 0)
		perror("F_GETFL error");
//	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags|O_NONBLOCK) < 0)
		perror("F_SETFL error");
}

myEpoll::myEpoll(unsigned short port)
	: sock(port)
{
	
}

myEpoll::~myEpoll()
{
//	Epoll_ctl(epollfd, EPOLL_CTL_DEL, 0, NULL);
	Epoll_ctl(epollfd, EPOLL_CTL_DEL, listen_sock, &ev);
	close(epollfd);
}

void
myEpoll::set_handler( epollFun *func )
{
	handler = func;
}

int
myEpoll::Epoll_create(int size)
{
	int epfd = epoll_create(size);
	if (epfd == -1) {
	    perror ("epoll_create");
	    exit(EXIT_FAILURE);
	}
	return epfd;
}

int
myEpoll::Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	int err = epoll_ctl( epfd, op, fd, event);
	if (err == -1) {
		perror("epoll_ctl: listen/conn_sock");
		exit(EXIT_FAILURE);
	}
	return err;
}

int
myEpoll::Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	int nfds = epoll_wait( epfd, events, maxevents, timeout);
	if (nfds == -1) {
		perror("epoll_pwait");
		exit(EXIT_FAILURE);
	}
	return nfds;
}

void
myEpoll::start()
{
//	sock.Socket(AF_INET, SOCK_STREAM, 0);
	
	listen_sock = sock.getListen();
	
	sock.Bind(listen_sock);
	
	sock.Listen(listen_sock);
	// FIXME Я не считаю таки обертки (своя ОО обертка вокруг каждой C-функции) правильными...
	// Сделать из пары C++ и glibc подобие Python - не является смоцелью, а просто тратой времени
	// и усложнение кода.
	// Проще в одном методе (типа этого) позвать сразу все Сишные функции с проверками результата
	// и выбросом исклчений - так Си код изолируется от C++ кода без потери 'красоты', но
	// экономией времени.
	// Если нужны OO сокеты, то есть Boost::asio... Хуже того, ОО сокеты вообще никому не нужны
	// на самом деле, а нужны уже готовые паттерны типа реактора и проактора ( с уже реализованным
	// epoll и пр.)
	
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	
	epollfd = Epoll_create(MAX_EVENTS);
	
	Epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev);
	
}

void
myEpoll::loop()
{
	if(!handler){
		perror("loop: do_use_fd no defined");
	}
	for (;;) {
		nfds = Epoll_wait(epollfd, events, MAX_EVENTS, -1);
		int n;
		for (n = 0; n < nfds; ++n) {
			if (events[n].data.fd == listen_sock) {
				
				conn_sock = sock.Accept(listen_sock);

				std::cout << "new client: " << sock.Sock_ntop() << std::endl;
				
				sock.setnonblocking(conn_sock);
				write(conn_sock, hello, strlen(hello) ); //!!aghtung send helloworld!!
		
		//		do_use_fd(conn_sock); //! ololo del
				
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				
				if( Epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) < 0) {
					perror("loop: Epoll_ctl");
				}
				
			} else {
				// by analogy with True branch, epoll ok
				printf("READY\n");
				//! do use fd
				(*handler) (events[n].data.fd);
				// FIXME да, звать прикладную логику через хендлер - очень хорошее решение.
				// Примерно то, что делает Boost::asio и, может быть, libevent
				// Хотя... Не всегда это нужно...
				// Суть в том, что для маленьких проектов важна скорость разработки без
				// потери качества (легкости, надежности и производительности) кода.
				// Такие же generic-решения хороши для намного больших проектов.
				// Предугадываться же какие части будут использоватсья в другом проекти
				// или расширяться в существующем - это не всегда тревиальный вопрос.
			}
		}
	}
}

