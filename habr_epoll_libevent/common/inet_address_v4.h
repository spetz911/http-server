#ifndef __COMMON_INET_ADDRESS_V4_H__
#define __COMMON_INET_ADDRESS_V4_H__

#include "inet_address.h"

namespace Network
{
  
  class InetAddressV4
    : public InetAddress
  {
  public:
    static InetAddressV4 CreateFromString(std::string const &addr, unsigned short port);
    
  private:
    InetAddressV4(sockaddr const *addr, int size);
  };
  
}

#endif  // !__COMMON_INET_ADDRESS_V4_H__
