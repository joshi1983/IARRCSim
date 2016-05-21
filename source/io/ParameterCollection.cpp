#include "ParameterCollection.hpp"

#include <cstring>
#include <iostream>

using namespace std;

void ParameterCollection::add(const char *name,const char *val)
{
	if (name==NULL || val==NULL)
	{
	   cout << 
		   "ParameterCollection::add failed due to NULL name or value."
		   <<endl;
	   return;
	}
  parameters.push_back(Parameter(name,val));
}

const std::list<Parameter>& ParameterCollection::getParameters() const
{
  return parameters;
}

const char* ParameterCollection::getAsCharPtr(const char *name) const
{
  list<Parameter>::const_iterator it = parameters.begin();

   for (;it!=parameters.end();it++)  
   {
      if (strcmp(it->getName(),name)==0)
         return it->getValue();
   }

   return NULL;
}

void ParameterCollection::remove(const char*name)
{
  list<Parameter>::iterator it = parameters.begin();

   for (;it!=parameters.end();it++)  
   {
      if (strcmp(it->getName(),name)==0)
      {
         parameters.erase(it);
         break;  
      }
   }
  
}

void ParameterCollection::clear()
{
  parameters.clear();
}

