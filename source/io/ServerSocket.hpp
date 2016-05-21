#ifndef SERVER_SOCKET_HEADER
#define SERVER_SOCKET_HEADER

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <winsock2.h>

#endif

#include "Socket.hpp"

class Socket;
class DocServer;
void handleIncomingRequests(ServerSocket *ss);

class ServerSocket
{
 friend class DocServer;
 friend void handleIncomingRequests(ServerSocket *ss);
 
 private:
   int port;

  #ifdef _UBUNTU_
  
   int listenSocketDescriptor;
   /* my address information */
   struct sockaddr_in my_addr;
  
  #else
  
   SOCKET listenSocket;
   struct sockaddr_in sin;
  
  #endif

   bool startServer();

 public:
   ServerSocket(int port);
   ~ServerSocket();
   Socket * acceptClient(); // for accepting a connection
   int getPort() const;
   void closeSocket();
};

#endif
