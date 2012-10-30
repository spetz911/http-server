#include "tcp_server.h"
#include "tcp_server_socket.h"
#include "selector_thread.h"
#include "logger.h"

#include <map>

#include <assert.h>

namespace Network
{

  namespace Private
  {
  
    class ListenThread
      : private TCPServerSocket
      , public Common::IDisposable
    {
    public:
      ListenThread(InetAddress const &locAddr, int backlog,
                   ClientItemQueuePtr acceptedClients,
                   UserSessionCreator sessionCreator)
        : TCPServerSocket(locAddr, backlog)
        , AcceptedClients(acceptedClients)
        , SessionCreator(sessionCreator)
        , Selector(1, WaitTimeout, std::tr1::bind(&ListenThread::OnSelect,
            this, std::tr1::placeholders::_1, std::tr1::placeholders::_2))
      {
        Selector.AddSocket(GetHandle(), Network::ISelector::stRead);
      }
      
    private:
      enum { WaitTimeout = 20 };
      ClientItemQueuePtr AcceptedClients;
      UserSessionCreator SessionCreator;
      SelectorThread Selector;
      
      void OnSelect(SocketHandle handle, Network::ISelector::SelectType selectType)
      {
        try
        {
          SocketHolder NewConnection;
          sockaddr Addr = { 0 };
          socklen_t AddrSize = sizeof(Addr);
          Accept(&NewConnection, true, &Addr, &AddrSize);
          
          std::auto_ptr<ClientItem> Client(new ClientItem(NewConnection,
            InetAddress::CreateFromSockAddr(&Addr, AddrSize),
            SessionCreator()));
  
          if (!AcceptedClients.get())
            throw std::invalid_argument("Empty destQueue pointer");
          
          AcceptedClients->Push(Client);
        }
        catch (std::exception const &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
      }
    };
  
    class WorkerThread
      : private Common::NonCopyable
      , public Common::IDisposable
    {
    public:
      WorkerThread(int maxEventsCount, ClientItemQueuePtr acceptedClients)
        : MaxConnections(maxEventsCount)
        , AcceptedClients(acceptedClients)
        , Selector(maxEventsCount, WaitTimeout, std::tr1::bind(&WorkerThread::OnSelect,
              this, std::tr1::placeholders::_1, std::tr1::placeholders::_2),
            SelectorThread::ThreadFunctionPtr(new SelectorThread::ThreadFunction(std::tr1::bind(
              &WorkerThread::OnIdle, this))))
      {
      }
      
    private:
      enum { WaitTimeout = 20 };
      
      typedef std::map<SocketHandle, ClientItemPtr> ClientPool;
      unsigned MaxConnections;
      ClientItemQueuePtr AcceptedClients;
      ClientPool Clients;
      SelectorThread Selector;
      
      void OnSelect(SocketHandle handle, Network::ISelector::SelectType selectType)
      {
        try
        {
          ClientPool::iterator Iter = Clients.find(handle);
          assert(Iter != Clients.end() && "Client not found");
          if (Iter == Clients.end())
            return;
          switch (selectType)
          {
          case Network::ISelector::stRead :
            {
              try
              {
                Iter->second->RecvData();
              }
              catch (...)
              {
                RemoveClient(Iter);
                throw;
              }
            }
            return;
          case Network::ISelector::stClose :
              RemoveClient(Iter);
            return;
          default :
            break;
          }
        }
        catch (std::exception const &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
      }
      
      void OnIdle()
      {
        try
        {
          for (ClientPool::iterator i = Clients.begin() ; i != Clients.end() ; )
          {
            if (i->second->CanClose())
              RemoveClient(i++);
            else
            {
              try
              {
                i->second->OnIdle();
                if (i->second->CanClose())
                  RemoveClient(i++);
                else
                  ++i;
              }
              catch (std::exception const &e)
              {
                Clients.erase(i++);
                Common::Log::GetLogInst() << e.what();
              }
            }
          }
          
          if (Clients.size() >= MaxConnections)
            return;
          
          ClientItemPtr Client = AcceptedClients->Pop();
          if (!Client.get())
            return;
          
          Selector.AddSocket(Client->GetHandle(), Network::ISelector::stRead | Network::ISelector::stClose);
          Clients[Client->GetHandle()] = Client;
        }
        catch (std::exception const &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
      }
      
      void RemoveClient(ClientPool::iterator iter)
      {
        try
        {
          Selector.DelSocket(iter->second->GetHandle());
        }
        catch (std::exception const &e)
        {
          Common::Log::GetLogInst() << e.what();
        }
        Clients.erase(iter);
      }
    };

  }
  
  TCPServer::TCPServer(InetAddress const &locAddr, int backlog, int maxThreadsCount,
            int maxConnectionsCount, UserSessionCreator sessionCreator)
    : AcceptedItems(new Private::ClientItemQueue(backlog))
  {
    int EventsCount = maxConnectionsCount / maxThreadsCount;
    for (int i = 0 ; i < maxThreadsCount ; ++i)
    {
      Threads.push_back(IDisposablePtr(new Private::WorkerThread(
          EventsCount + (i <= maxThreadsCount - 1 ? 0 : maxConnectionsCount % maxThreadsCount),
          AcceptedItems
        )));
    }
    Threads.push_back(IDisposablePtr(new Private::ListenThread(locAddr, backlog, AcceptedItems, sessionCreator)));
  }

}
