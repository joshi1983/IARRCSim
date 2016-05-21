#include "Job.hpp"

Job::Job(): done(false),destroyDelayed(false)
{
}

void Job::setDone(bool done)
{
  this->done = done;
}

bool Job::isDone() const
{
  return done;
}

bool Job::isDestroyDelayed() const
{
  return destroyDelayed;
}

void Job::delayDestroy()
{
  destroyDelayed=true;
}

void Job::doJob()
{
  run();
  done=true;
}


