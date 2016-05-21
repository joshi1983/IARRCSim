#include "SingleVariableValue.hpp"

using namespace std;

SingleVariableValue::SingleVariableValue(const string &value): value(value)
{
}

string SingleVariableValue::getAsString() const
{
    return value;
}

bool SingleVariableValue::arrayAdd( VariableValue *value)
{
  return false;
}

VariableValue * SingleVariableValue::getClone() const
{
  return new SingleVariableValue(this->value);
}