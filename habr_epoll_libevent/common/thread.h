#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "exceptions.h"
#include "non_copyable.h"

#include <tr1/functional>
#include <tr1/memory>

#include <pthread.h>

namespace System
{
  
  DECLARE_RUNTIME_EXCEPTION(Thread)
  
  class Thread
    : private Common::NonCopyable
  {
  public:
    typedef std::tr1::function<void ()> ThreadFunction;
    typedef std::auto_ptr<ThreadFunction> ThreadFunctionPtr;
    
    Thread(ThreadFunction const &func);
    ~Thread();
    
  private:
    ThreadFunctionPtr Function;
    pthread_t Handle;
    
    static void* ThreadFunc(void *prm);
  };
  
}

#endif  // !__COMMON_THREAD_H__
