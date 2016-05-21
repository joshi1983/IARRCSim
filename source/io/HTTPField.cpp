#include "HTTPField.hpp"

using namespace std;

HTTPField::HTTPField(string name,string value):name(name),value(value)
{

}

const string & HTTPField::getName() const
{
  return name;
}

const string & HTTPField::getValue() const
{
  return value;
}

void HTTPField::writeTo(ostream &out) const
{
	out << name << ": "<<value<<endl;
}

void HTTPField::writeTo(Socket &s) const
{
   s.write(name.c_str(),name.size()); 
   s.write(": ",2);
   s.write(value.c_str(),value.size());
   s.write("\r\n",2);
}
