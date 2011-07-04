#ifndef __THREAD2_CC__
#define __THREAD2_CC__
// FIXME эти штуки пишутся для инклюдников на случай если один инклюдник A инклюдится
// в файлах B и С, а те в свою очередь в D - чтобы не было конфликтов в такос случае.

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
sock_write_all( int sockfd, FILE *file )
{
//	std::string line;
	char *line = NULL;
	size_t len;
	
	
//	while(std::getline( file , line)) {
	while (getline(&line, &len, file) > 0){
		std::cout << "danger read" << std::endl;
		
		write(sockfd, line, len );
		std::cout << "danger writ" << std::endl;
		// memorry lost here (line)
		if (line != NULL){
			free(line);
			line = NULL;
		}
		
	}
	std::cout << "continue" << std::endl;
	
}

void
sock_write_404( int sockfd )
{
	std::string error404 ("HTTP/1.1 404 Not Found \r\n");
	write(sockfd, error404.c_str(), error404.length() );
	// FIXME std::string() для этих целей совсем не нужен.
	// Вполне сработало бы write(sockfd, "HTTP/1.1 404 Not Found \r\n",
	// 				sizeof("HTTP/1.1 404 Not Found \r\n") - 1);
	// Только саму строку надо было вынести в дефайн.
	// Таким образом длина строки вычислилась бы компиляторм, а не в runtime,
	// да еще и с аллокацией памяти внутри самого std::string().
}
void
sock_write_200( int sockfd )
{
	std::string error404 ("HTTP/1.1 200 OK\r\n\r\n");
	write(sockfd, error404.c_str(), error404.length() );
}



void
send_file(const std::string& path, int sockfd)
{
//	std::string line;
	std::cout << "send file before open" << std::endl;
	
	int fd_open = open(path.c_str(), O_RDONLY, "r");
	if (fd_open > 0){
		sendfile(sockfd, fd_open, NULL, 100500);
		// FIXME использовать sendfile() - это очень правильно.
		// Но не правильно использовать 'левые' значения для длинны
		// - лучше было динамически определить размер файла.
		// На худой конец - написать коммент откуда взялось 100500
	}
	return;
}

void
cgi_run(const std::string& path, const int sockfd)
{
	myPipe cgi(path);
	
	if (cgi.lookup(path)) {
//		int fd = cgi.doFork();
		cgi.doFork();
		
//! fdopen( int fd, mode); !//		

		sock_write_all( sockfd, stdin );
		cgi.close_cin();
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
	char* pattern = (char *) argv1;
	myPCRE parser(pattern);
	
	std::string path;
	std::string msg;
	std::string rec;
	
	while(42){
		myData *data = process_q.pop();
		std::cout << "pop ok" << std::endl;

		std::string msg = std::string(data->getMsg());
		// FIXME имена типа getMsg() тоже раньше использовал.
		// Код читается так хуже (имена воспринимаются хуже).
		// Предпочтительнее get_msg() для функций, file_path для переменных
		// и WorkQueue только для имен структур и классов.
		int sockfd = data->getfd();
		
		path = parser.test(msg);
		
		std::cout << "parser.test.ok" << std::endl;
		
		if (path == ""){
			// FIXME Если паттерн не соответствует файлу, то возвращается NULL из parser.test()
			// NULL и "" - это разные, не равные, вещи
			sock_write_404( sockfd );
			// FIXME по дизайну совсем не красиво.
			// Что есди мы захотим использовать этот же код для другого проекта (SMTP, например)?
			// Мы хтим использовать только очереди и файловые операции, но протокол у нас другой.
			// В общем случае это должно выглядеть примерно так (имена примерные):
			// 1. в начале создается объект класса Daemon (это вся многопоточность и network IO)
			// 2. Daemon - это или шаблон и просто несет в себе класс HttpProcessor
			//    (возможно у него мембером указатель на абстрактный класс ProtocolProcessor,
			//     от которого и наследуется HttpProcessor)
			// 3. HttpProcessor - принимает на вход считанные в Daemon буфферы данных и удерживает
			//    для каждого сокета свой конечный автомат (состояния get_request, process_request,
			//    send_response и пр.)
			// 4. HttpProcessor в свою очередь связн с классом фильтров Filter (где и лежит PCRE
			//    - в данном случае - это myPCRE parser) и классом файловых операций.
			//
			// Из метода обработки очереди сразу звать HTTP-specific логику совсем не правильно.
		}
		else{
			std::cout << "parser.test.ok" << std::endl;
			sock_write_200( sockfd );
			send_file(path, sockfd);
		}
		
		
	//!	cgi_run(path, sockfd);
	
		std::cout << "thread2.ok" << std::endl;
		
		const char* msg1 = data->getMsg();
		if (msg1!=NULL){
			delete[] msg1;
			// FIXME Да! Это здорово, то что надо - все удаляется один раз и без копирования.
			// Пара замечаний:
			// 1. в прниципе лучше было запретить копирование для mydata и удалять строку
			//    внутри ее деструктора
			// 2. в идеале удалять структуры ровно в той функции, в которой они были созданы
			//    (по карайней мере, выделение и удаление желательно располагать на одном
			//    логическом уровне) - проекты становятся сложнее и разобраться с кодом
			//    становится со временем все сложнее
			data->setMsg(NULL);
			msg1=NULL;
		}
		delete data;

	}

}

#endif
