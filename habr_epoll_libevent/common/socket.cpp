#include "socket.h"
#include "inet_address.h"

#include <errno.h>

namespace Network
{
  
  Socket::Socket(Type type, bool nonBlocking)
    : SocketHolder(socket(AF_INET, GetSockType(type) | (nonBlocking ? SOCK_NONBLOCK : 0), 0))
  {
  }
  
  void Socket::Bind(InetAddress const &addr)
  {
    if (bind(GetHandle(), addr.GetAddr(), addr.GetSize()) == -1)
      throw SocketException("Failed to bind socket", errno);
  }
  
  void Socket::Listen(int backlog)
  {
    if (listen(GetHandle(), backlog) == -1)
      throw SocketException("Failed to listen socket", errno);
  }
  
  void Socket::Accept(SocketHolder *newSocket, bool nonBlocking,
                      sockaddr *newAddr, socklen_t *newAddrSize)
  {
    if (!newSocket)
      throw SocketException("Invalid input parameter");
    if (newSocket->IsValid())
      throw SocketException("Input holder must be not initialized");
    SocketHolder NewSocket(accept4(GetHandle(), newAddr, newAddrSize, nonBlocking ? SOCK_NONBLOCK : 0));
    newSocket->Swap(NewSocket);
  }
  
  int Socket::GetSockType(Type type)
  {
    switch (type)
    {
    case Tcp :
      return SOCK_STREAM;
    case Udp :
      return SOCK_DGRAM;
    default :
      break;
    }
    throw SocketException("Unknown socket type");
  }

}
