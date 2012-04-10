#include "epoll.hpp"

#include "tqueue.h"

#include "thread_pool.h"

// FIXME
// *.cc - это исходники (они же *.cpp - это просто кто как привык)
// Инклюдники - *.h (или *.hpp, опять же кто как привык)
// Мы используем *.cc и *.h
// см. коментарии в mydata.cc
#include <stdexcept>

#include <iostream>

const int MAXLINE = 4096;

const size_t MAX_NUM_THREADS = 20;

// producer-consumer pattern
ThreadQueue<int> process_q(7);
#include "thread2.cc"

char *argv1 = NULL;

// FIXME
// функции, которые в других *.cc не используются лучше объявлять static
// (это актуально больше для C, чем C++, но так легче разбираться в коде)

static void
use_fd(int sockfd)
{
	process_q.push(sockfd);
}



int
main(int argc, char *argv[])
{
	try {
		// Check command line arguments.
		if (argc != 5) {
			std::cerr << "Usage: http_server <host> <port> <threads> <doc_root>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 80 1 .\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 80 1 .\n";
			return 1;
		}
	
		std::string host(argv[1]);
		unsigned short port = atoi(argv[2]);
		
		size_t num_threads = atoi(argv[3]);
		if (num_threads < 1 || num_threads > MAX_NUM_THREADS) {
			throw std::logic_error("wrong num threads");
		}
		std::string doc_root(argv[4]);
		
		std::cerr << "ThreadPool\n";
		
		// set worker threads
		ThreadPool tpool(num_threads);
		tpool.set_args(doc_root);
		tpool.run(&wait_on_queue);
		
		std::cerr << "myEpoll\n";
		
		// set main thread
		myEpoll epoll(port);
		epoll.set_handler(&use_fd);
		epoll.start();
		epoll.loop();
	
	} catch (std::exception &e) {
		std::cerr << "exception: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "unknown exception" << std::endl;
	}
	return 0;
}


