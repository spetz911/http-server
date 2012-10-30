#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__

#include "exceptions.h"
#include "non_copyable.h"

#include <pthread.h>

namespace System
{

  DECLARE_RUNTIME_EXCEPTION(Mutex)
  
  class Mutex
    : private Common::NonCopyable
  {
  public:
    Mutex();
    ~Mutex();
    void Lock();
    void Unlock();
    
  private:
    pthread_mutex_t Handle;
  };
  
}

#endif  // !__COMMON_MUTEX_H__
