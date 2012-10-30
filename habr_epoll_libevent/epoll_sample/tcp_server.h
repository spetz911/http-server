#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "non_copyable.h"
#include "iuser_session.h"
#include "inet_address.h"
#include "idisposable.h"
#include "queue.h"
#include "client_item.h"
#include "mutex.h"

#include <tr1/functional>
#include <tr1/memory>

#include <vector>

namespace Network
{

  namespace Private
  {
  
    typedef Queue<ClientItem, System::Mutex> ClientItemQueue;
    typedef std::tr1::shared_ptr<ClientItemQueue> ClientItemQueuePtr;
    
  }
  
  typedef std::tr1::function<IUserSessionPtr ()> UserSessionCreator;

  class TCPServer
    : private Common::NonCopyable
  {
  public:
    TCPServer(InetAddress const &locAddr, int backlog, int maxThreadsCount,
              int maxConnectionsCount, UserSessionCreator sessionCreator);
    
  private:
    typedef std::tr1::shared_ptr<Common::IDisposable> IDisposablePtr;
    typedef std::vector<IDisposablePtr> IDisposablePool;
    Private::ClientItemQueuePtr AcceptedItems;
    IDisposablePool Threads;
  };
  
}

#endif  // !__TCP_SERVER_H__
