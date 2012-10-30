#include "inet_address_v4.h"

#include <arpa/inet.h>```

namespace Network
{
  
  InetAddressV4 InetAddressV4::CreateFromString(std::string const &addr, unsigned short port)
  {
    sockaddr_in SockAddr = { 0 };
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    SockAddr.sin_port = htons(port);
    InetAddressV4 Addr(reinterpret_cast<sockaddr const *>(&SockAddr), sizeof(SockAddr));
    return Addr;
  }
    
  InetAddressV4::InetAddressV4(sockaddr const *addr, int size)
    : InetAddress(addr, size)
  {
  }

}
