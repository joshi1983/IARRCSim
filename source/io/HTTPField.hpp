#ifndef HTTPFIELD_HEADER
#define HTTPFIELD_HEADER

#include <string>
#include <iostream>

#include "Socket.hpp"

class HTTPField
{
 private:
   std::string name;
   std::string value;

 public:
   HTTPField(std::string name,std::string value);

   const std::string & getName() const;
   const std::string & getValue() const;   
   void writeTo(std::ostream &out) const;
   void writeTo(Socket &s) const;
};

#endif