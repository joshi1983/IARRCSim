#include "NamedValue.hpp"

using namespace std;

NamedValue::NamedValue(const std::string &name,VariableValue *vv):
 name(name),value(vv)
{

}

NamedValue::~NamedValue()
{
 if (value!=NULL)
 {
   // delete value;
   // should this be deleted?  is it safe to delete it?
 }
}

const VariableValue* NamedValue::getValue() const
{
  return value;
}

const string& NamedValue::getName() const
{
    return name;
}


const string NamedValue::getValueAsString() const
{
     return value->getAsString();
}