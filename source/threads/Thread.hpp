#ifndef THREAD_HEADER
#define THREAD_HEADER

#include "../stdafx.h"

#ifdef _UBUNTU_

#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */

#else

#include <windows.h>

#endif

class Thread
{
 private:

#ifdef _UBUNTU_

  pthread_t threadHandle;

#else

   HANDLE threadHandle;

#endif

   void *param;
   void (*runFunc)(void *);

 public:
   Thread(void (*runFunc)(void *),void *param);
   void start();
   static long long getCurrentTickCount();
   static void sleepMillis(int millis);
   static void startThread(void (*runFunc)(void *),void *param);
};

#endif
