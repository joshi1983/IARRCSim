#include "EnvironmentRemovePhysicalObjectJob.hpp"


EnvironmentRemovePhysicalObjectJob::EnvironmentRemovePhysicalObjectJob(
	Environment *env,PhysicalObject*po,bool deleteObjectMemory):env(env),po(po),deleteObjectMemory(deleteObjectMemory)
{

}

void EnvironmentRemovePhysicalObjectJob::run()
{
  env->remove(po);

  if (deleteObjectMemory)
     delete po;
}
