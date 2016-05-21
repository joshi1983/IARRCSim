#ifndef VARIABLE_COLLECTION_HEADER
#define VARIABLE_COLLECTION_HEADER

#include "NamedValue.hpp"

#include <string>
#include <list>

class VariableCollectionStack;

class VariableCollection
{
	friend class VariableCollectionStack;

private:
	std::list<NamedValue*> variables;

    NamedValue* getNamedValueByName(const std::string &name);

public:
   VariableCollection();
   virtual ~VariableCollection();

     void clear();
    std::string getValueAsStringFor(const std::string &name) const;

    const VariableValue* getVariableValueByName(
	const std::string &name) const;

    void add(NamedValue *v);

};

#endif