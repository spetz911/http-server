#ifndef __COMMON_SOCKET_HOLDER_H__
#define __COMMON_SOCKET_HOLDER_H__

#include "exceptions.h"
#include "non_copyable.h"
#include "types.h"

namespace Network
{
  
  DECLARE_RUNTIME_EXCEPTION(SocketHolder)
  
  class SocketHolder
    : private Common::NonCopyable
  {
  public:
    SocketHolder();
    SocketHolder(SocketHandle handle);
    virtual ~SocketHolder();
    SocketHandle GetHandle() const;
    bool IsValid();
    void Swap(SocketHolder &holder);
    
  private:
    SocketHandle Handle;
  };

}

#endif  // !__COMMON_SOCKET_HOLDER_H__
