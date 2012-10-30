#include "socket_holder.h"

#include <unistd.h>
#include <sys/socket.h>

namespace Network
{
  
  SocketHolder::SocketHolder()
    : Handle(InvalidSocket)
  {
  }
  
  SocketHolder::SocketHolder(SocketHandle handle)
    : Handle(handle)
  {
    if (Handle == InvalidSocket)
      throw SocketHolderException("Invalid socket");
  }
  
  SocketHolder::~SocketHolder()
  {
    if (IsValid())
    {
      shutdown(Handle, SHUT_RDWR);
      close(Handle);
    }
  }
  
  SocketHandle SocketHolder::GetHandle() const
  {
    return Handle;
  }
  
  bool SocketHolder::IsValid()
  {
    return Handle != InvalidSocket;
  }
  
  void SocketHolder::Swap(SocketHolder &holder)
  {
    SocketHandle Tmp = Handle;
    Handle = holder.Handle;
    holder.Handle = Tmp;
  }

}
