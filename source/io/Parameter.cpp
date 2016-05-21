#include "Parameter.hpp"

#include <cstring>

using namespace std;

Parameter::~Parameter()
{
  delete [] value;
}

Parameter::Parameter(const char *name,const char *value)
{
  strcpy(this->name,name);
  this->value = new char[strlen(value)+1];
  strcpy(this->value,value);
}

Parameter::Parameter(const Parameter& param)
{
  strcpy(this->name,param.name);
    this->value = new char[strlen(param.value)+1];
  strcpy(this->value,param.value);
}

Parameter & Parameter::operator=(const Parameter& param)
{
  strcpy(this->name,param.name);

  delete [] this->value;
  this->value = new char[strlen(param.value)+1];
  strcpy(this->value,param.value);

 return *this;
}

const char * Parameter::getName() const
{
  return name;
}

const char * Parameter::getValue() const
{
  return value;
}




