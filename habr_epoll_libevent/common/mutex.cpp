#include "mutex.h"

#include <errno.h>
#include <assert.h>

namespace System
{

  Mutex::Mutex()
    : Handle(pthread_mutex_t())
  {
    if (pthread_mutex_init(&Handle, 0))
      throw MutexException("Failed to create mutex", errno);
  }
  
  Mutex::~Mutex()
  {
    int Res = pthread_mutex_destroy(&Handle);
    assert(Res == 0 && "Failed to destroy mutex");
  }
  
  void Mutex::Lock()
  {
    if (pthread_mutex_lock(&Handle))
      throw MutexException("Failed to mutex lock", errno);
  }
  
  void Mutex::Unlock()
  {
    if (pthread_mutex_unlock(&Handle))
      throw MutexException("Failed to mutex unlock", errno);
  }

}
