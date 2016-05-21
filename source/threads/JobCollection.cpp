#include "JobCollection.hpp"

using namespace std;

// some thread synchronization must be
// implemented here.

JobCollection::~JobCollection()
{
   clear();
}

void JobCollection::clear()
{
  jobsListMutex.enterCriticalSection();

  list<Job*>::iterator it = jobs.begin();

  for (;it!=jobs.end();it++)
  {
	  if (!(*it)->isDestroyDelayed())
          delete (*it);
  }
  jobs.clear();

  jobsListMutex.leaveCriticalSection();
}

void JobCollection::add(Job *j)
{
    jobsListMutex.enterCriticalSection();

  jobs.push_back(j);

  jobsListMutex.leaveCriticalSection();

}

void JobCollection::doJobs()
{
 jobsListMutex.enterCriticalSection();

  list<Job*>::iterator it = jobs.begin();

  for (;it!=jobs.end();it++)
  {
     (*it)->doJob();
  }

  clear();

  jobsListMutex.leaveCriticalSection();
}
