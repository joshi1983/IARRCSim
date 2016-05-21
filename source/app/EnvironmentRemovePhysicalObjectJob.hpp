#ifndef ENVIRONMENT_REMOVEPHYSICAL_OBJECT_JOB_HEADER
#define ENVIRONMENT_REMOVEPHYSICAL_OBJECT_JOB_HEADER

#include "Environment.hpp"
#include "../physicalobjects/PhysicalObject.hpp"
#include "../threads/Job.hpp"

/**
A class for storing a removal job.

This is valuable when you want to get the main
thread to remove an object from the environment.

*/
class EnvironmentRemovePhysicalObjectJob: public Job
{
private:
  Environment *env;
  PhysicalObject*po;
  bool deleteObjectMemory;

protected:

	virtual void run();

public:
   EnvironmentRemovePhysicalObjectJob(Environment *env,PhysicalObject*po,bool deleteMemory);

};


#endif
