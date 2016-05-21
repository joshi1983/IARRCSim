#ifndef PARAMETER_COLLECTION_HEADER
#define PARAMETER_COLLECTION_HEADER

#include <list>
#include "Parameter.hpp"

class ParameterCollection
{
 private:
  std::list<Parameter> parameters;

 public:
   const std::list<Parameter>& getParameters() const;
   void add(const char *name,const char *val);
   const char* getAsCharPtr(const char *name) const;
   void remove(const char*name);
   void clear();
};

#endif