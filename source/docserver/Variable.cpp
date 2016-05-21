#include "Variable.hpp"
#include "SingleVariableValue.hpp"
#include "ArrayVariableValue.hpp"
#include <iostream>

using namespace std;

Variable::Variable(const string &name):
  NamedValue(name,NULL)
{
}

Variable::Variable(const std::string &name,VariableValue *vv):
  NamedValue(name,vv)
{
  
}

void Variable::assign(VariableValue *value)
{
  if (this->value!=NULL)
	  delete this->value;

  this->value=value;
}


void Variable::assign(const string &value)
{
	if (this->value!=NULL)
		delete this->value;

     this->value = new SingleVariableValue(value);
}

void Variable::arrayAdd(VariableValue *value)
{
    if (this->value==NULL)
    {
           this->value=new ArrayVariableValue(value);
    }
    else if (!this->value->arrayAdd(value))
    {
       this->value=new ArrayVariableValue(value);
	  this->value->arrayAdd(value);
    }

}


