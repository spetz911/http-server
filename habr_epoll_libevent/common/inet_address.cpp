#include "inet_address.h"

#include <string.h>

namespace Network
{

  InetAddress::InetAddress(InetAddress const &addr)
    : Addr(0)
    , Size(0)
  {
    Init(addr.GetAddr(), addr.GetSize());
  }
  
  InetAddress& InetAddress::operator = (InetAddress const &addr)
  {
    InetAddress Tmp(addr);
    Swap(Tmp);
    return *this;
  }
  
  InetAddress::~InetAddress()
  {
    ::operator delete (Addr);
  }
  
  sockaddr const* InetAddress::GetAddr() const
  {
    return Addr;
  }
  
  int InetAddress::GetSize() const
  {
    return Size;
  }
  
  void InetAddress::Swap(InetAddress &addr)
  {
    sockaddr *TmpAddr = addr.Addr;
    addr.Addr = Addr;
    Addr = TmpAddr;
    int TmpSize = addr.Size;
    addr.Size = Size;
    Size = TmpSize;
  }
  
  InetAddress InetAddress::CreateFromSockAddr(sockaddr *addr, int size)
  {
    InetAddress Addr(addr, size);
    return Addr;
  }
  
  InetAddress::InetAddress(sockaddr const *addr, int size)
    : Addr(0)
    , Size(0)
  {
    Init(addr, size);
  }
  
  void InetAddress::Init(sockaddr const *addr, int size)
  {
    if (!addr || !size)
      throw InetAddressException("Invalid address");
    Addr = reinterpret_cast<sockaddr *>(::operator new(size));
    Size = size;
    memcpy(Addr, addr, Size);
  }

}
