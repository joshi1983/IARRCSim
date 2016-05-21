#ifndef CONSTANT_HEADER
#define CONSTANT_HEADER

#include "NamedValue.hpp"

class Constant: public NamedValue
{

 public:
	 Constant(const std::string &name,const std::string &val);

  virtual void assign(const std::string &value);
  virtual void assign(VariableValue *value);
  virtual void arrayAdd(VariableValue *value);

};


#endif