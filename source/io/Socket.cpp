#include "Socket.hpp"

#include <iostream>

#ifdef _UBUNTU_

#include <cstdlib>


#define SOCKET_ERROR -1

#else

#ifdef _USE_VISUAL_STUDIO_
#include <ws2tcpip.h>

// link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#else

#include <winsock.h>
#define NI_MAXSERV  512
#define NI_MAXHOST 512

#endif

#endif

#include <cstdio>

using namespace std;

bool Socket::initialized=false;

#ifdef _UBUNTU_

Socket::Socket(int s):s(s)
{

}

#else

Socket::Socket(SOCKET s):s(s)
{
	if (!init())
	{
		cout << "Socket::init failed."<<endl;
		throw 2;
	}
}

#endif

Socket::Socket(const char *host,int port)
{
	if (!init())
	{
		cout << "Socket::init failed."<<endl;
		throw 2;
	}
    if (!connectToServer(host,port))
       throw 1;
}

bool Socket::connectToServer(const char *hostname,int port)
{
 #ifdef _UBUNTU_

   struct hostent *he;
   struct sockaddr_in their_addr;

 // may have to convert "localhost" into "127.0.0.1"

 // get the host info
 if((he=gethostbyname(hostname)) == NULL)
 {
    cerr<<"gethostbyname()"<<endl;
    return false;
 }

 if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
 {
    cerr<<"socket()"<<endl;
    return false;
 }

 // host byte order

 their_addr.sin_family = AF_INET;

 // short, network byte order
 their_addr.sin_port = htons(port);
 their_addr.sin_addr = *((struct in_addr *)he->h_addr);

 // zero the rest of the struct
 memset(&(their_addr.sin_zero), '\0', 8);

 if (connect(s, (struct sockaddr *)&their_addr,
   sizeof(struct sockaddr)) == -1)
 {
    cerr<<"connect()"<<endl;
    return false;
 }



 #else // Windows

  struct sockaddr_in sin;

#ifdef _USE_VISUAL_STUDIO_

 int error;
 char servInfo[NI_MAXSERV];
 long addr=inet_addr(hostname);

  if (addr==INADDR_NONE)
  {
    cout << "Invalid IP address: "<<hostname<<endl;
    return false;
  }
  cout << "Got addr: "<<addr<<endl;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = addr;
  sin.sin_port = htons( port );

  cout << "Going to getnameinfo."<<endl;

  char hname[NI_MAXHOST];
  strcpy(hname,hostname);


    /* resolve the domain name into a list of addresses */
  error = getnameinfo((struct sockaddr *) &sin,
                           sizeof (struct sockaddr),
                           (PCHAR)hname,
                           NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

    if (error != 0)
    {
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
        return false;
    }

   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (s == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError() );
        return 1;
    }

  if ( connect( s, (PSOCKADDR)&sin, sizeof sin ) == SOCKET_ERROR )
  {
      /* could not connect to server */
      printf("Error at connect(): %ld\n", WSAGetLastError() );
      return false;
  }

#else

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr( hostname );
    sin.sin_port = htons( port );

    //cout<<"CONNECTED"<<endl;

    if ( connect( s, (SOCKADDR*) &sin, sizeof( sin ) ) == SOCKET_ERROR )
    {
        cerr<<"ClientSocket: Failed to connect\n";
       // system("pause");
        WSACleanup();
    }

    #endif

  #endif


  return true;
}

int Socket::read(void *buf,int numBytes)
{
  if (buf==NULL)
  {
	  cout <<
	  "Socket::read failed.  Unable to store in NULL buffer."<<endl;
	  return 0;
  }
  return recv(s,(char*)buf,numBytes,0);
}

bool Socket::write(const void *buf,int numBytes)
{
   if (send(s,(char*)buf,numBytes,0)==SOCKET_ERROR)
	   return false;

   return true;
}

void Socket::closeWriteEnd()
{
 #ifndef _UBUNTU_

   shutdown(s, SD_SEND);

 #endif
}

void Socket::closeReadEnd()
{
 #ifndef _UBUNTU_

  shutdown(s, SD_RECEIVE);

 #endif
}


void Socket::closeSocket()
{
  #ifdef _UBUNTU_

  close(s);

  #else

   closeWriteEnd();
   closeReadEnd();
   closesocket( s );

  #endif
}

Socket::~Socket()
{
  closeSocket();
}

bool Socket::init()
{
 #ifndef _UBUNTU_

  if (initialized)
	return true;

   WSADATA wsaData={0};
   int iResult;

   iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
	   printf("Error at Socket::init(): %ld\n", WSAGetLastError() );
        return false;
    }

    //cout << "WSA initialized."<<endl;

  initialized=true;

/* WinSock has been initialized */

  #endif

  return true;
}


void Socket::shutdownSocketSystem()
{
  #ifndef _UBUNTU_

  WSACleanup();

  #endif

  initialized=false;
}
