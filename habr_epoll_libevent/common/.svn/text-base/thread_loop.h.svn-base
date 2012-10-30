#ifndef __COMMON_THREAD_LOOP_H__
#define __COMMON_THREAD_LOOP_H__

#include "thread.h"

#include <memory>

namespace System
{

  class ThreadLoop
    : private Common::NonCopyable
  {
  public:
    ThreadLoop(Thread::ThreadFunction const &func);
    ~ThreadLoop();
    
  private:
    bool volatile IsRun;
    typedef std::auto_ptr<Thread> ThreadPtr;
    Thread::ThreadFunctionPtr Function;
    ThreadPtr Worker;
    
    void Work();
  };

}

#endif  // !__COMMON_THREAD_LOOP_H__
