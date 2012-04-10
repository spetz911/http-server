#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <string>
#include <vector>
#include <pthread.h>

#include <stdexcept>

class ThreadPool {
private:
	std::vector<pthread_t> thread_pool;
	std::string doc_root;

public:
	ThreadPool(size_t num)
		: thread_pool(num)
	{}
	
	void
	set_args(const std::string &str)
	{
		doc_root = str;
	}
	
	void
	run(void *(*start_routine) (void *)) throw(std::exception)
	{
		int err;
		for (size_t i=0; i < thread_pool.size(); ++i) {
		
			err = pthread_create(&thread_pool[i], NULL, start_routine, (void *)this);
			switch (err) {
			case EAGAIN:
				throw std::logic_error("Insufficient resources to create thread");
			case EINVAL:
				throw std::logic_error("Invalid settings in attr");
			case EPERM:
				throw std::logic_error("No permission to set the scheduling\
				                      policy and parameters specified in attr");
			}
			
			err = pthread_detach(thread_pool[i]);
			switch (err) {
			case EINVAL:
				throw std::logic_error("Thread is not a joinable thread");
			case ESRCH:
				throw std::logic_error("No thread with the ID thread could be found");
			}
		}
	
	}
};


#endif //__THREAD_POOL_H__
