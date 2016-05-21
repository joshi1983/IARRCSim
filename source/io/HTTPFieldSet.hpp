#ifndef HTTPFIELD_SET_HEADER
#define HTTPFIELD_SET_HEADER

#include <list>
#include <iostream>
#include "HTTPField.hpp"
#include "Socket.hpp"

class HTTPFieldSet
{
 private:
   std::list<HTTPField> fields;

 public:
   void clear();
   void add(const std::string &name,const std::string &value);
   const std::string * getValueFor(const std::string &name) const;
   void remove(const std::string & fieldName);   
   void writeTo(std::ostream & out) const;
   void writeTo(Socket &s) const;
};

#endif