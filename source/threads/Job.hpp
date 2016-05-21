#ifndef JOB_HEADER
#define JOB_HEADER

class Job
{
private:
	// false until this job is executed.
	bool done;

	// true only when the Job is not to be destroyed by the execution thread.
	bool destroyDelayed;

protected:
	virtual void run()=0;

 public:
   Job();
   void setDone(bool done);
   bool isDone() const;
   bool isDestroyDelayed() const;
   void delayDestroy();
   void doJob();
};

#endif
