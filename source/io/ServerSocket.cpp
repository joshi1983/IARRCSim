#include "ServerSocket.hpp"

#ifdef _UBUNTU_

#define MAXPENDING 10
/* Max connection requests */

#endif

#include <cstdio>

using namespace std;

ServerSocket::ServerSocket(int port): port(port)
{
  if (!Socket::init())
  {
	cout << "Socket::init failed."<<endl;
	throw 2;
  }
  if (!startServer())
  {
    throw 1;
  }

}

Socket * ServerSocket::acceptClient()
{
 #ifdef _UBUNTU_

 /* Listen on the server socket */
  if (listen(listenSocketDescriptor, MAXPENDING) < 0)
  {
     cerr << "Failed to listen on server socket"<<endl;

     switch (errno)
    {
      case EADDRINUSE:
       cout << "Another socket is already listening on the same port."<<endl;
       break;
      case EBADF:
       cout <<
       "The argument listenSocketDescriptor is not a valid descriptor: "
       <<listenSocketDescriptor<<endl;
       break;
      case ENOTSOCK:
       cout << "The argument listenSocketDescriptor is not a socket."<<endl;
       break;
      case EOPNOTSUPP:
       cout << "The socket is not of a type that "
        <<"supports the listen() operation. "<<endl;
	break;
      default:
        cout << "Unknown error code number: "<<errno<<endl;
    }
     return NULL;
  }

  struct sockaddr_in echoclient;
  int acceptedDescriptor;
  unsigned int clientlen = sizeof(echoclient);

  if ((acceptedDescriptor =
                   accept(listenSocketDescriptor,
		    (struct sockaddr *) &echoclient,
                          &clientlen)) < 0)
  {
       cerr<<"Failed to accept client connection"<<endl;
       return NULL;
  }

  return new Socket(acceptedDescriptor);

 #else

  SOCKET AcceptSocket;
 // printf("Waiting for client to connect...\n");

  //----------------------
  // Accept the connection.
  AcceptSocket = accept( listenSocket, NULL, NULL );
  if (AcceptSocket == INVALID_SOCKET) {
    printf("accept failed: %d\n", WSAGetLastError());
    return NULL;
  } else
  {
	return new Socket(AcceptSocket);
  }

 #endif
}

int ServerSocket::getPort() const
{
  return port;
}

bool ServerSocket::startServer()
{

 #ifdef _UBUNTU_

  if((listenSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    cerr<<"Server-socket() listenSocketDescriptor error!"<<endl;

    return false;
  }

  /* host byte order */
  my_addr.sin_family = PF_INET;

  /* short, network byte order */
  my_addr.sin_port = htons(port);

  /* automatically fill with my IP */
  my_addr.sin_addr.s_addr = INADDR_ANY;

  /* zero the rest of the struct */
  memset(&(my_addr.sin_zero), '\0', 8);

  if (bind(listenSocketDescriptor, (struct sockaddr *)&my_addr,
    sizeof(struct sockaddr)) == -1)
  {
    cerr<<"Server-bind() error!"<<endl;
    cerr<<"The port may be in use by another process.  If so, there are 2 options:"<<endl;
    cerr<<"   1. Identify the process using port "<<port<<" and take it off."<<endl;
    cerr<<"   2. Configure this simulator to use another port."<<endl;
    cerr<<"If you are using Linux, the following command may help identify what processes are using the port."<<endl;
    cerr<<"lsof -w -n -i tcp:"<<port<<endl;
    cerr<<"To change the port used by this server, "
    <<"edit the docserver-port property in config/config.ini."<<endl;

    perror("Check that the port is not already in use.\n");
    return false;
  }

  return true;

 #else
  // windows

  memset( &sin, 0, sizeof sin );

  listenSocket= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    return false;
  }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  sin.sin_port = htons( port );

  if (bind( listenSocket,
    (SOCKADDR*) &sin,
    sizeof(sin)) == SOCKET_ERROR)
  {
    printf("bind failed: %d\n", WSAGetLastError());
    return false;
  }

  //----------------------
  // Listen for incoming connection requests.
  // on the created socket
  if (listen( listenSocket, 1 ) == SOCKET_ERROR) {
    printf("Error listening on socket: %d\n", WSAGetLastError());
    return 1;
  }

  return true;

  #endif
}

void ServerSocket::closeSocket()
{
  #ifdef _UBUNTU_

  close(listenSocketDescriptor);

  #else

  closesocket( listenSocket );
  //server=NULL;

  #endif
}

ServerSocket::~ServerSocket()
{
  closeSocket();
}
