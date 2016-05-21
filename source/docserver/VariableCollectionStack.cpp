#include "VariableCollectionStack.hpp"
#include "SingleVariableValue.hpp"
#include "ArrayVariableValue.hpp"
#include "Variable.hpp"

#include "../app/Environment.hpp"
#include <iostream>

using namespace std;

void VariableCollectionStack::arrayAdd(const std::string &name,
							    const std::string &valText)
{
  NamedValue* var=getNamedValueByName(name);
  VariableValue* valueRef=new SingleVariableValue(valText);

  if (var!=NULL)
     var->arrayAdd(valueRef);
  else
  {
    ArrayVariableValue *val = new ArrayVariableValue();
    val->arrayAdd(valueRef);
    this->setOrDeclare(name,val);
  }
}

NamedValue* VariableCollectionStack::getNamedValueByName(
	const std::string &name)
{
  for (int i=scopes.size()-1;i>=0;i--)
  {
	  NamedValue* result=scopes[i]->getNamedValueByName(name);

	   if (result!=NULL)
	   {
	     return result;
	   }
  }
  return NULL;
}

const VariableValue* VariableCollectionStack::getVariableValueByName(
	const std::string &name) const
{
  for (int i=scopes.size()-1;i>=0;i--)
  {
	  const VariableValue* result=scopes[i]->getVariableValueByName(name);

	   if (result!=NULL)
	   {
	     return result;
	   }
  }
  return NULL;
}

string VariableCollectionStack::getValueAsStringFor(const string &name)
{
	// go from deepest scope up.
     for (int i=scopes.size()-1;i>=0;i--)
	{
	  string result=scopes[i]->getValueAsStringFor(name);

	   if (result!="")
	   {
	     return result;
	   }
	}
	const string result=Environment::getEnvironmentProperty(name);

	if (result=="")
	{
	 //  cout << "Undeclared variable or property '"<<name<<"'"<<endl;
	}
	return result;
}

void VariableCollectionStack::declareNamedValue(NamedValue *v)
{
	if (scopes.size()==0)
	{
	   pushEmptyScope();
	}
     scopes[scopes.size()-1]->add(v);
}

void VariableCollectionStack::setOrDeclare(const string&name,VariableValue *val)
{
  NamedValue *v1=getNamedValueByName(name);
  Variable *v=dynamic_cast<Variable*>(v1);

  if (v==NULL)
  {
	  v=new Variable(name,val);
	  declareNamedValue(v);
	  if (v1!=NULL)
		  delete v1;
  }
  else
  {
	  v->assign(val);
  }

}

void VariableCollectionStack::pushEmptyScope()
{
	VariableCollection *vc=new VariableCollection();

     scopes.push_back(vc);
}

void VariableCollectionStack::popScope()
{
	// destroy the last VariableCollection.
   VariableCollection*vc=scopes[scopes.size()-1];

     scopes.pop_back();

     delete vc;
}

void VariableCollectionStack::push(VariableCollection *c)
{
	if (c==NULL)
		pushEmptyScope();
	else
         scopes.push_back(c);
}
