#include "VariableCollection.hpp"
#include <iostream>

using namespace std;

VariableCollection::VariableCollection()
{

}

VariableCollection::~VariableCollection()
{
	  clear();
}

void VariableCollection::clear()
{
	    list<NamedValue*>::iterator it = variables.begin();
	   
	    for (;it!=variables.end();it++)
	    {
		    delete *it;
	    }

	    variables.clear();
}

NamedValue* VariableCollection::getNamedValueByName(const std::string &name)
{
	    list<NamedValue*>::iterator it = variables.begin();

	    for (;it!=variables.end();it++)
	    {
		    if ((*it)->getName()==name)
			    return *it;
	    }

	    return NULL;
}

const VariableValue* VariableCollection::getVariableValueByName(
	const std::string &name) const
{
	    list<NamedValue*>::const_iterator it = variables.begin();

	    for (;it!=variables.end();it++)
	    {
	       if ((*it)->getName()==name)
			  return (*it)->getValue();
	    }

	    return NULL;
}

string VariableCollection::getValueAsStringFor(const string &name) const
{
	    list<NamedValue*>::const_iterator it = variables.begin();

	    for (;it!=variables.end();it++)
	    {
	       if ((*it)->getName()==name)
			  return (*it)->getValueAsString();
	    }
	    return string();
}

void VariableCollection::add(NamedValue *v)
{
	if (v!=NULL)
        variables.push_back(v);
}
