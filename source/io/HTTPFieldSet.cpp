#include "HTTPFieldSet.hpp"

using namespace std;

bool case_insensitiveEquals(const string &s1,const string &s2)
{
  if (s1.length()!=s2.length())
	return false;

  for (int i=0;i<s1.length();i++)
  {
	  if (tolower(s1[i])!=tolower(s2[i]))
		  return false;
  }

  return true;
}

void HTTPFieldSet::clear()
{
  fields.clear();
}

void HTTPFieldSet::add(const string &name,const string &value)
{
  remove(name);
  fields.push_back(HTTPField(name,value));
}

const string * HTTPFieldSet::getValueFor(const string &name) const
{
  // loop through fields.
  list<HTTPField>::const_iterator it = fields.begin();

   for (;it!=fields.end();it++)
   {
      if (case_insensitiveEquals(it->getName(),name))
         return &(it->getValue());
   }

  return NULL; // indicate not found.
}

void HTTPFieldSet::remove(const string & fieldName)
{
   list<HTTPField>::iterator it = fields.begin();

   for (;it!=fields.end();it++)
   {
      if (case_insensitiveEquals(it->getName(),fieldName))
	 {
         fields.erase(it);
	    break;
	 }
   }

}  

void HTTPFieldSet::writeTo(ostream & out) const
{
   // loop through fields.
  list<HTTPField>::const_iterator it = fields.begin();

   for (;it!=fields.end();it++)
   {
      it->writeTo(out);
   } 
}

void HTTPFieldSet::writeTo(Socket &s) const
{
  list<HTTPField>::const_iterator it = fields.begin();

   for (;it!=fields.end();it++)
   {
      it->writeTo(s);
   } 
}