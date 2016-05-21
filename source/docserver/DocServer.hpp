#ifndef DOC_SERVER_HEADER
#define DOC_SERVER_HEADER

#include "../stdafx.h"
#include "../io/ServerSocket.hpp"
#include "../threads/Thread.hpp"

class DocServer
{
 private:
   ServerSocket ss;

   static int port;
   
 public:

  DocServer();

  static void setDocServerPort(int port);
  static int getDocServerPort();
};

#endif
