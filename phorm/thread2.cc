#ifndef __THREAD2_CC__
#define __THREAD2_CC__


#include <string>
#include <pthread.h>
#include <stdio.h>

#include "epoll.hpp"

#include <limits.h>
#include "tqueue.h"
#include <stdexcept>

#include "http_parser.h"


/* TODO
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
*/

ssize_t
Read(int fd, void *buf, size_t count) throw(std::exception)
{
	ssize_t len;
	len = read(fd, buf, count);
	if (len == -1) {
		switch(errno) {
			default:
				throw std::logic_error("read exept");
		}
	}
	return len;
}
/*
       EAGAIN The  file descriptor fd refers to a file other than a socket and
              has been marked nonblocking (O_NONBLOCK),  and  the  read  would
              block.

       EAGAIN or EWOULDBLOCK
              The  file  descriptor  fd refers to a socket and has been marked
              nonblocking   (O_NONBLOCK),   and   the   read   would    block.
              POSIX.1-2001  allows  either error to be returned for this case,
              and does not require these constants to have the same value,  so
              a portable application should check for both possibilities.

       EBADF  fd is not a valid file descriptor or is not open for reading.

       EFAULT buf is outside your accessible address space.

       EINTR  The  call  was interrupted by a signal before any data was read;
              see signal(7).

       EINVAL fd is attached to an object which is unsuitable for reading;  or
              the  file  was  opened  with  the  O_DIRECT flag, and either the
              address specified in buf, the value specified in count,  or  the
              current file offset is not suitably aligned.

       EINVAL fd  was  created  via  a call to timerfd_create(2) and the wrong
              size buffer was given to read(); see timerfd_create(2) for  fur‐
              ther information.


       EIO    I/O  error.  This will happen for example when the process is in
              a background process group, tries to read from  its  controlling
              tty,  and  either  it  is  ignoring  or  blocking SIGTTIN or its
              process group is orphaned.  It may also occur when  there  is  a
              low-level I/O error while reading from a disk or tape.

       EISDIR fd refers to a directory.
*/

#define MSG_MAX 666

void
process_request(int sockfd) throw(std::exception)
{
	// volatile
	ssize_t len;
	char recvline[MSG_MAX]; // SSIZE_MAX
/*
	read():
	On success, the number of bytes read is returned
	(zero indicates end of file).
*/

#ifdef PHORM_DEBUG
	len = Read(sockfd, &recvline, MSG_MAX);
#else
	len = read(sockfd, &recvline, MSG_MAX);
	if (len == -1)
		return; // FIXME return error_code
#endif
	recvline[len] = '\0'; // may be
	
	std::cout << "receive.test:" << std::endl;
	std::cout << recvline << std::endl;
	
	
	HttpParser parser(recvline, MSG_MAX);
	
	parser.write_response_header(sockfd);
	parser.write_file("Untitled", sockfd);
	
	// path2 = parser.test(path);
	
	char *path = NULL;
	
	if (path == NULL) {
//		sock_write_404( sockfd );
	}
	else {
//		sock_write_200( sockfd );
//		send_file(path, sockfd);
	}

}

void *
wait_on_queue(void *argv1)
{
	//! second thread

	for (;;) {
		int sockfd = process_q.pop();
		process_request(sockfd);
		
		std::cout << "thread2.ok" << std::endl;
	}
	return 0;
}

#endif //__THREAD2_CC__


