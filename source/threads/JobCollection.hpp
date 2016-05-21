#ifndef JOB_COLLECTION_HEADER
#define JOB_COLLECTION_HEADER

#include <list>
#include "Job.hpp"
#include "Mutex.hpp"

class JobCollection
{
 private:

   std::list<Job*> jobs;
   Mutex jobsListMutex;

 public:

   virtual ~JobCollection();
   void add(Job *j);
   void doJobs();
   void clear();
};

#endif
