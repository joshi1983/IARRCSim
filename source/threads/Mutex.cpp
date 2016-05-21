#include "Mutex.hpp"

#include <iostream>

using namespace std;

Mutex::Mutex()
{
  initialize();
}

Mutex::~Mutex()
{
 #ifndef _UBUNTU_

  DeleteCriticalSection(&cs);

 #endif

}

void Mutex::initialize()
{
 #ifndef _UBUNTU_

  InitializeCriticalSection(&cs);

 #endif

}

void Mutex::enterCriticalSection()
{
 #ifdef _UBUNTU_

    pthread_mutex_lock( &cs );

 #else

    EnterCriticalSection(&cs);

 #endif

 //cout << "Entered Critical Section."<<endl;
}

void Mutex::leaveCriticalSection()
{

 #ifdef _UBUNTU_

    pthread_mutex_unlock( &cs);

 #else

    LeaveCriticalSection(&cs);

 #endif

 //cout << "Left Critical Section."<<endl;
}
