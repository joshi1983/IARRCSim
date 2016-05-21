#ifndef VARIABLE_VALUE_HEADER
#define VARIABLE_VALUE_HEADER

#include <string>

class VariableValue
{
public:
   virtual std::string getAsString() const=0;
   virtual bool arrayAdd(VariableValue *value)=0;
   virtual VariableValue * getClone() const=0;
};

#endif