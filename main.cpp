#include "epoll.hpp"
#include "pcre.hpp"
#include "pipe.hpp"

#include "mydata.cc"
#include "tqueue.cc"
// FIXME
// *.cc - это исходники (они же *.cpp - это просто кто как привык)
// Инклюдники - *.h (или *.hpp, опять же кто как привык)
// Мы используем *.cc и *.h
// см. коментарии в mydata.cc

#include <iostream>

const int MAXLINE = 4096;

ThreadQueue< myData *> process_q(7);
#include "thread2.cc"

char *argv1 = NULL;

// FIXME
// функции, которые в других *.cc не используются лучше объявлять static
// (это актуально больше для C, чем C++, но так легче разбираться в коде)
void
use_fd( int sockfd)
{
	char* pattern = (char *) argv1;
	myPCRE parser(pattern);
	// FIXME
	// лучше было бы завести всего один myPCRE объект и не тратить время на компиляцию
	// выражения каждый раз.
	// parser может быть или глобальным объектом-синглтоном (не здорово) либо
	// один раз создаваться в начале и далее передаваться по указателю/ссылке
	// Или быть мембером некоторого класса (вроде Processor - класс который принимает
	// соединения, парсит данные и вызывает методы класса FileSystem для загрузки файла -
	// например, так....)
	
	char recvline[MAXLINE];
	myData *data = new myData;
	char *path;
	
	volatile int len;
	
	if( (len = read(sockfd, &recvline, MAXLINE )) >0){
		recvline[len] = '\0'; //may be
		
		path = parser.test(recvline);
		
		std::cout << "parser.test.ok" << std::endl;
		std::cout << recvline << std::endl;
		
		data->setMsg(path); // set recvline here
		data->setfd (sockfd);
		std::cout << "push" << std::endl;
		process_q.push( data);
		
		std::cout << "thread1.ok" << std::endl;
	}
	return;
}

int
main(int argc, char *argv[])
{
	//take pcre pattern
	char dafault_p[] = "[at]";
//	char *argv1;
	
	switch (argc) {
	case 1 :
		argv1 = dafault_p; // pattern (regexp)
		break;
	case 2:
		argv1 = argv[1];
		break; 
	default:
		std::cerr << "too many args" << std::endl;
	}
	// set second thread
	pthread_t t1;
	pthread_create(&t1, NULL, &waitOnQueue, NULL); // (void *)argv1);
	pthread_detach(t1);
	
	// set first thread
	myEpoll epoll;
	epoll.setHandler( &use_fd);
	epoll.start();
	epoll.loop();
	
	return 0;
}
