#ifndef __TQUEUE_H__
#define __TQUEUE_H__

#include <list>
#include <queue>
#include <pthread.h>
#include <boost/noncopyable.hpp>

/*
 * Serialized work queue.
 */
 
template<class T>
class ThreadQueue : private boost::noncopyable {
private:
	unsigned int		overflow;
	pthread_cond_t		cond;
	pthread_mutex_t		wait_mtx;
	std::queue<T, std::list<T> > queue;

public:
	ThreadQueue(unsigned int o) throw();
	~ThreadQueue() throw();

	bool push(T& x) throw();
	T pop() throw();
};


template<class T>
ThreadQueue<T>::ThreadQueue(unsigned int o) throw()
	: overflow(o)
{
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&wait_mtx, NULL);
}

template<class T>
ThreadQueue<T>::~ThreadQueue() throw()
{
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&wait_mtx);
}

template<class T>
bool
ThreadQueue<T>::push(T& x) throw()
{
	pthread_mutex_lock(&wait_mtx);
	queue.push(x);
	bool o = (queue.size() >= overflow);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&wait_mtx);

	return o;
}

template<class T>
T
ThreadQueue<T>::pop() throw()
{
	pthread_mutex_lock(&wait_mtx);
	while (queue.empty())
		pthread_cond_wait(&cond, &wait_mtx);
	T x = queue.front();
	queue.pop();
	pthread_mutex_unlock(&wait_mtx);

	return x;
}

#endif //__TQUEUE_H__
