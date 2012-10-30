#include "client_item.h"
#include "io_helper.h"

namespace Network
{

  ClientItem::ClientItem(SocketHolder &holder, InetAddress const &addr, IUserSessionPtr userSession)
    : InetAddress(addr)
    , SessionLastActionTime(std::time(0))
    , MarkedForClose(false)
    , UserSession(userSession)
    , BufferSize(userSession->GetMaxBufSizeForRead())
    , RecvBuffer(new char [BufferSize])
  {
    SocketHolder::Swap(holder);
    UserSession->Init(this);
  }
  
  ClientItem::~ClientItem()
  {
    UserSession->Done();
    delete [] RecvBuffer;
  }
  
  bool ClientItem::CanClose() const
  {
    return MarkedForClose;
  }
  
  void ClientItem::RecvData()
  {
    if (MarkedForClose)
      return;
    IOHelper IO(GetHandle());
    for (unsigned Bytes = BufferSize ; IO.Read(RecvBuffer, &Bytes) ; Bytes = BufferSize)
      UserSession->OnRecvData(RecvBuffer, Bytes);
  }
  
  void ClientItem::OnIdle()
  {
    if (!MarkedForClose)
      MarkedForClose = UserSession->IsExpiredSession(SessionLastActionTime);
    if (MarkedForClose)
      return;
    UserSession->OnIdle();
  }
  
  void ClientItem::MarkMeForClose()
  {
    MarkedForClose = true;
  }
  
  void ClientItem::UpdateSessionTime()
  {
    SessionLastActionTime = std::time(0);
  }
  
  bool ClientItem::SendData(void const *buf, unsigned *bytes)
  {
    IOHelper IO(GetHandle());
    return IO.Write(buf, bytes);
  }
  
  bool ClientItem::SendFile(int fileHandle, unsigned offset, unsigned *bytes)
  {
    IOHelper IO(GetHandle());
    return IO.SendFile(fileHandle, offset, bytes);
  }
  
  InetAddress const& ClientItem::GetAddress() const
  {
    return *this;
  }
  
  SocketTuner ClientItem::GetSocketTuner() const
  {
    return SocketTuner(GetHandle());
  }

}
