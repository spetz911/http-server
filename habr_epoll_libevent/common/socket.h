#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include "socket_holder.h"

#include <sys/socket.h>

namespace Network
{

  class InetAddress;
  
  DECLARE_RUNTIME_EXCEPTION(Socket)

  class Socket
    : public SocketHolder
  {
  public:
    enum Type { Tcp, Udp };
    
    Socket(Type type, bool nonBlocking);
    void Bind(InetAddress const &addr);
    void Listen(int backlog);
    void Accept(SocketHolder *newSocket, bool nonBlocking,
                sockaddr *newAddr = 0, socklen_t *newAddrSize = 0);
    
  private:
    static int GetSockType(Type type);
  };
  
}

#endif  // !__COMMON_SOCKET_H__
