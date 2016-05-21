#ifndef SOCKET_HEADER
#define SOCKET_HEADER

/*
TIP: Include Socket.hpp before any other include.

Most importantly, include Socket.hpp before including anything that directly or indirectly 
includes winsock.h and windows.h is one of the winsock includers.  
In other words, include Socket.hpp above #include <windows.h>

If you don't, you'll see lots of "redefined" error messages when compiling.
*/
#include "../stdafx.h"

#ifdef _UBUNTU_

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#else

#include <winsock2.h>
#define _WINSOCK2API_
#define _WINSOCKAPI_ 

#endif

#include <iostream>

class ServerSocket;

class Socket
{
 friend class ServerSocket;

 private:
 #ifdef _UBUNTU_
 

   int s;
 
 #else
 
     SOCKET s;

 #endif

 private:
 
 #ifdef _UBUNTU_
 
     Socket(int descriptor);
 
 #else
	Socket(SOCKET s);

 #endif
 
	bool connectToServer(const char *host,int port);

 public:
   Socket(const char *host,int port);
   ~Socket();
   int read(void *buf,int numBytes);
   bool write(const void *buf,int numBytes);
   void closeWriteEnd();
   void closeReadEnd();
   void closeSocket();

   static bool initialized;

   /*
   init must be called before using Socket and ServerSocket classes.
   */
   static bool init();

   /*
   To be called when finished using sockets
   */
   static void shutdownSocketSystem();
};

#endif
