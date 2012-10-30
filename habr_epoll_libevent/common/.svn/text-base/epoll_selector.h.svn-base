#ifndef __COMMON_EPOLL_SELECTOR_H__
#define __COMMON_EPOLL_SELECTOR_H__

#include "exceptions.h"
#include "non_copyable.h"
#include "iselector.h"

#include <sys/epoll.h>

#include <vector>

namespace Network
{
  
  DECLARE_RUNTIME_EXCEPTION(EPollSelector)
  
  class EPollSelector
    : private Common::NonCopyable
    , public ISelector
  {
  public:
    EPollSelector(int maxSocketCount);
    ~EPollSelector();
    virtual void AddSocket(SocketHandle handle, int selectType);
    virtual void DelSocket(SocketHandle handle);
    virtual void Select(SelectFunction *function, unsigned timeout);
    
  private:
    typedef std::vector<epoll_event> EventPool;
    EventPool Events;
    int EPoll;
    
    static int GetSelectFlags(int selectType);
  };

}

#endif  // !__COMMON_EPOLL_SELECTOR_H__
