#ifndef VARIABLE_HEADER
#define VARIABLE_HEADER

#include "NamedValue.hpp"


class Variable: public NamedValue
{

public:
  Variable(const std::string &name);
  Variable(const std::string &name,VariableValue *vv);

  virtual void assign(const std::string &value);
  virtual void assign(VariableValue *value);
  virtual void arrayAdd(VariableValue *value);

};

#endif