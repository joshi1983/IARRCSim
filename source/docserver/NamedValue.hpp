#ifndef NAMED_VALUE_HEADER
#define NAMED_VALUE_HEADER

#include "VariableValue.hpp"
#include <string>

class NamedValue
{
 protected:
     std::string name;
     VariableValue *value;

 public:
  NamedValue(const std::string &name,VariableValue *vv);
  virtual ~NamedValue();

  const VariableValue* getValue() const;
  const std::string getValueAsString() const;
  const std::string& getName() const;

  virtual void assign(const std::string &value)=0;
  virtual void assign(VariableValue *value)=0;
  virtual void arrayAdd(VariableValue *value)=0;
};

#endif
