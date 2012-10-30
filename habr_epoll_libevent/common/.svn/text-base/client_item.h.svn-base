#ifndef __COMMON_CLIENT_ITEM_H__
#define __COMMON_CLIENT_ITEM_H__

#include "socket_holder.h"
#include "inet_address.h"
#include "iconnection_ctrl.h"
#include "iuser_session.h"

#include <ctime>

#include <tr1/memory>

namespace Network
{

  class ClientItem
    : public SocketHolder
    , public InetAddress
    , public IConnectionCtrl
  {
  public:
    ClientItem(SocketHolder &holder, InetAddress const &addr, IUserSessionPtr userSession);
    ~ClientItem();
    
    bool CanClose() const;
    void RecvData();
    void OnIdle();
    
    // IConnectionCtrl
    virtual void MarkMeForClose();
    virtual void UpdateSessionTime();
    virtual bool SendData(void const *buf, unsigned *bytes);
    virtual bool SendFile(int fileHandle, unsigned offset, unsigned *bytes);
    virtual InetAddress const& GetAddress() const;
    virtual SocketTuner GetSocketTuner() const;
    
  private:
    std::time_t SessionLastActionTime;
    bool MarkedForClose;
    IUserSessionPtr UserSession;
    unsigned BufferSize;
    char *RecvBuffer;
  };
  
  typedef std::tr1::shared_ptr<ClientItem> ClientItemPtr;

}

#endif  // !__COMMON_CLIENT_ITEM_H__
