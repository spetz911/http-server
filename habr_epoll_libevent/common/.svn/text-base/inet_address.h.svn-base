#ifndef __COMMON_INET_ADDRESS_H__
#define __COMMON_INET_ADDRESS_H__

#include "exceptions.h"
#include "non_copyable.h"

#include <sys/socket.h>

namespace Network
{
  
  DECLARE_RUNTIME_EXCEPTION(InetAddress)
  
  class InetAddress
    : private Common::NonCopyable
  {
  public:
    InetAddress(InetAddress const &addr);
    InetAddress& operator = (InetAddress const &addr);
    virtual ~InetAddress();
    sockaddr const* GetAddr() const;
    int GetSize() const;
    void Swap(InetAddress &addr);
    static InetAddress CreateFromSockAddr(sockaddr *addr, int size);
    
  protected:
    InetAddress(sockaddr const *addr, int size);
    
  private:
    sockaddr *Addr;
    int Size;
    
    void Init(sockaddr const *addr, int size);
  };

}

#endif  // !__COMMON_INET_ADDRESS_H__
