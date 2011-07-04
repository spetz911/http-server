#ifndef __THREAD2_CC__
#define __THREAD2_CC__

#include <sys/sendfile.h>

#include <string>
#include <pthread.h>
#include <stdio.h>

#include "epoll.hpp"
#include "pcre.hpp"
#include "pipe.hpp"

#include "mydata.cc"
#include "tqueue.cc"

void
sock_write_404( int sockfd )
{
	std::string error404 ("HTTP/1.1 404 Not Found \r\n");
	write(sockfd, error404.c_str(), error404.length() );
}
void
sock_write_200( int sockfd )
{
	std::string error200 ("HTTP/1.1 200 OK\r\n\r\n");
	write(sockfd, error200.c_str(), error200.length() );
}
void
sock_write_eof( int sockfd )
{
	std::string error200 ("file not found\r\n");
	write(sockfd, error200.c_str(), error200.length() );
}



void
send_file(const char *path, int sockfd)
{
//	std::cout << "send file, before open" << std::endl;
	int fd_open = open(path, O_RDONLY, "r");
	if (fd_open > 0){
		sendfile(sockfd, fd_open, NULL, 100500);
	}
	else {
		sock_write_eof(fd_open);
	}
	return;
}

void
cgi_run(char* path, int sockfd)
{

	myPipe cgi(path); //path
	
	//~ execvp(argv2[0], argv2);
	//~ perror("vse ploho");
	//~ exit(1);
	
	int fd_open;
	if (cgi.lookup(path)) {
		// FIXME насколько понял цели - здесь вполне можно было позвать popen()
		// и обойтись без самостоятельных вызовов fork(), execve() и пр.
		cgi.doFork();
		fd_open = cgi.get_cin();
		std::cerr << "sending file..." << std::endl;
		cgi.send_file(sockfd, fd_open, 100500);
		cgi.doWait();
	}
	else {
		sock_write_404( sockfd );
		return;
	}
	return;
}


void *
waitOnQueue(void *argv1){// throw()
	//! second thread

	myPCRE parser("\\w\\.py");
	char *path2 = NULL;
	
	while(42){
//FIXME
// 1. while (42) выглядит неприлично. Лучше использовать while (1) или for (;;)
// 2. Старайся писать (скобки, пробелы) в соответсвтвии с linux/Documentation/CodingStyle:
// 	a. фигурные скобки открытия функции ставятся в начале следующей строки
// 	b. между оператором while, круглыми скобками и фигурной ставятся проебелы -> while (1) {
// 	c. после имен функции перед скобками пробелы не ставим (у тебя правильно)
// 	и т.д.
		myData *data = process_q.pop();
		
		const char *path = data->getMsg();
		int sockfd = data->getfd();
		if (path!=NULL){
			std::cout << "pop ok:" << path << std::endl;
		}
		
		path2 = parser.test(path);

		
		if(path2 != NULL){
			std::cout << "parser python ok:" << std::endl;
			std::cout << path2 << ";" << std::endl;
			sock_write_200( sockfd );
			cgi_run(path2, sockfd);
			delete[] path2;
			path2 = NULL;
		}
		else {
			if (path == NULL){
				sock_write_404( sockfd );
			}
			else {
				sock_write_200( sockfd );
				send_file(path, sockfd);
				delete[] path;
				data->setMsg(NULL);
				path=NULL;
			}
		}
		std::cout << "thread2.ok" << std::endl;
		delete data;
	}
}

#endif
