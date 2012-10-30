#include "thread.h"

#include <errno.h>
#include <assert.h>

namespace System
{
  
  Thread::Thread(ThreadFunction const &func)
    : Function(new ThreadFunction(func))
  {
    if (pthread_create(&Handle, 0,&Thread::ThreadFunc, Function.get()) == -1)
      throw ThreadException("Failed to create thread", errno);
  }
  
  Thread::~Thread()
  {
    int Res = pthread_join(Handle, 0);
    assert(Res != -1 && "Failed to join thread");
  }
    
  void* Thread::ThreadFunc(void *prm)
  {
    (*reinterpret_cast<ThreadFunction *>(prm))();
    return 0;
  }

}
