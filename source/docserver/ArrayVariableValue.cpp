#include "ArrayVariableValue.hpp"
#include <string>
#include <iostream>

using namespace std;

ArrayVariableValue::ArrayVariableValue()
{
}

ArrayVariableValue::ArrayVariableValue(const ArrayVariableValue & v)
{
  for (unsigned int i=0;i<v.values.size();i++)
  {
     values.push_back(v.values[i]->getClone());
  }
}

ArrayVariableValue::ArrayVariableValue(VariableValue *firstVal)
{
  values.push_back(firstVal);
}

ArrayVariableValue::ArrayVariableValue(ArrayVariableValue * avv):
  values(avv->values)
{

}

const VariableValue * ArrayVariableValue::operator[](int i) const
{
  if (i<0 || (unsigned int)i>=values.size())
  {
	  cout << "THTML error: Tried to access array subscript "<<i
	    <<" but that is out of range 0.."<<values.size()<<"."<<endl;
	  return NULL;
  }

  return values[i];
}

std::vector<VariableValue*>::size_type ArrayVariableValue::size() const
{
  return values.size();
}

bool ArrayVariableValue::arrayAdd(VariableValue *value)
{
	values.push_back(value);
	return true;
}

string ArrayVariableValue::getAsString() const
{
	  string result("(");

	  for (unsigned int i=0;i<values.size();i++)
	  {
		  if (i!=0)
			  result+=","+values[i]->getAsString();
		  else
                result+=values[i]->getAsString();
	  }

	  result+=")";
	  return result;
}

VariableValue * ArrayVariableValue::getClone() const
{
  return new ArrayVariableValue(*this);
}
