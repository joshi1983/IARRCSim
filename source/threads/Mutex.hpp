#ifndef MUTEX_HEADER
#define MUTEX_HEADER

#include "Thread.hpp"

class Mutex
{
 private:
   #ifdef _UBUNTU_

    pthread_mutex_t cs;

   #else

     CRITICAL_SECTION cs;

   #endif

   void initialize();

 public:
   Mutex();
   virtual ~Mutex();
   void enterCriticalSection();
   void leaveCriticalSection();
};

#endif