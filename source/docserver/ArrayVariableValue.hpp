#ifndef ARRAY_VARIABLE_VALUE_HEADER
#define ARRAY_VARIABLE_VALUE_HEADER

#include "VariableValue.hpp"
#include <vector>

class ArrayVariableValue: public VariableValue
{
private:
	std::vector<VariableValue*> values;
public:
	ArrayVariableValue();
	ArrayVariableValue(const ArrayVariableValue & v);
	ArrayVariableValue(VariableValue *firstVal);

	ArrayVariableValue(ArrayVariableValue * avv);

	std::vector<VariableValue*>::size_type size() const;
	const VariableValue * operator[](int i) const;
	virtual bool arrayAdd(VariableValue *value);

	std::string getAsString() const;
	virtual VariableValue * getClone() const;
};


#endif