#ifndef SINGLE_VARIABLE_VALUE
#define SINGLE_VARIABLE_VALUE

#include <string>
#include "VariableValue.hpp"

class SingleVariableValue: public VariableValue
{
private:
	std::string value;
public:
  SingleVariableValue(const std::string &value);
  virtual std::string getAsString() const;
  virtual bool arrayAdd( VariableValue *value);
  virtual VariableValue * getClone() const;
};


#endif