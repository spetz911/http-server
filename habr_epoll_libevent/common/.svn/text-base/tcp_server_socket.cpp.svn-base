#include "tcp_server_socket.h"
#include "socket_tuner.h"

namespace Network
{

  TCPServerSocket::TCPServerSocket(InetAddress const &locAddr, int backlog)
    : Socket(Socket::Tcp, true)
  {
    SocketTuner Tuner(GetHandle());
    Tuner.SetReuseAddr(true);
    Bind(locAddr);
    Listen(backlog);
  }

}
