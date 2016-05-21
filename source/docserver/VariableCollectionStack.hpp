#ifndef VARIABLE_COLLECTION_STACK_HEADER
#define VARIABLE_COLLECTION_STACK_HEADER

#include "VariableCollection.hpp"
#include "VariableValue.hpp"
#include <vector>

/**
 For multiple scopes
*/
class VariableCollectionStack
{
 private:
  std::vector<VariableCollection*> scopes;

  NamedValue* getNamedValueByName(const std::string &name);

 public:
   std::string getValueAsStringFor(const std::string &name);
   const VariableValue* getVariableValueByName(const std::string &name) const;
   void arrayAdd(const std::string &name,const std::string &valName);

   void setOrDeclare(const std::string& name,VariableValue *val);
   void declareNamedValue(NamedValue *v);
   void pushEmptyScope();
   void push(VariableCollection *c);
   void popScope();
};

#endif

