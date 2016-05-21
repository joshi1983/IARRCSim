#ifndef PARAMETER_HEADER
#define PARAMETER_HEADER

class Parameter
{
 private:
   char name[100];
   char *value;

 public:
	virtual ~Parameter();
   Parameter(const char *name,const char *value);
   Parameter(const Parameter& param);
   Parameter & operator=(const Parameter& param);
   const char * getName() const;
   const char * getValue() const;

};


#endif