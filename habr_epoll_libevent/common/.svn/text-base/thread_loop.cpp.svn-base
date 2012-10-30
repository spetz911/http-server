#include "thread_loop.h"

namespace System
{

  ThreadLoop::ThreadLoop(Thread::ThreadFunction const &func)
    : IsRun(true)
    , Function(new Thread::ThreadFunction(func))
  {
    Worker.reset(new Thread(std::tr1::bind(&ThreadLoop::Work, this)));
  }

  ThreadLoop::~ThreadLoop()
  {
    IsRun = false;
    Worker.reset(0);
  }
    
  void ThreadLoop::Work()
  {
    while (IsRun)
      (*Function.get())();
  }
  
}
