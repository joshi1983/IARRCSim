#include "Thread.hpp"
#include <iostream>

#ifdef _UBUNTU_

#include <ctime>
#include <unistd.h>

#endif

using namespace std;

Thread::Thread(void (*runFunc)(void *),void *param): runFunc(runFunc),param(param)
{

}

void Thread::start()
{
 #ifdef _UBUNTU_

  pthread_create(&threadHandle,NULL,(void *(*)(void *))&runFunc,(void *)NULL);

 #else

  threadHandle = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)runFunc, (LPVOID)param, 0, NULL);

 
//  WaitForSingleObject(threadHandle, INFINITE);

 #endif

}

#ifdef _UBUNTU_

void busySleep(int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());

}

#endif

void Thread::sleepMillis(int millis)
{
  #ifdef _UBUNTU_
 
    // needs to be tested.
    
     sleep(millis/1000);
     busySleep(millis%1000);

  #else

     Sleep(millis);

  #endif
}

#ifdef _UBUNTU_

struct funcParam
{
  void (*f)(void *);
  void * val;
  funcParam(void (*f)(void *),void *val):
     f(f),val(val)
  {}
};

void * func(void * valPtr)
{
  funcParam *f=(funcParam*)valPtr;

  f->f(f->val);

  return NULL;
}

#endif

long long Thread::getCurrentTickCount()
{
  #ifdef _UBUNTU_
  
    return (long long)(clock()*1000/CLOCKS_PER_SEC);
  
  #else
  
     return (long long)(GetTickCount());
     
  #endif
}

void Thread::startThread(void (*runFunc)(void *),void *param)
{
 #ifdef _UBUNTU_
 
   pthread_t threadHandle;
  
  if (runFunc==NULL)
  {
     cout << "Thread::startThread, can't start a thread using a NULL function pointer."<<endl;
     return;
  }
  funcParam fp(runFunc,param);

  pthread_create(&threadHandle,NULL,(void *(*)(void *))func,&fp);


 #else

  HANDLE threadHandle = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)runFunc, (LPVOID)param, 0, NULL);

 #endif
}
