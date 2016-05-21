#include "Constant.hpp"
#include "SingleVariableValue.hpp"

using namespace std;

Constant::Constant(const std::string &name,const string & val):
  NamedValue(name,new SingleVariableValue(val))
{

}

void Constant::assign(const std::string &value)
{
  // do nothing.
}

void Constant::assign(VariableValue *value)
{
  // do nothing.
}

void Constant::arrayAdd(VariableValue *value)
{
  // do nothing.
}
