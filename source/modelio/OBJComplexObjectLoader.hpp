#ifndef OBJMODEL_LOADER_HEADER
#define OBJMODEL_LOADER_HEADER

#include "ComplexObjectLoader.hpp"

class OBJComplexObjectLoader: public ComplexObjectLoader
{

 public:
   virtual ComplexObject* loadFrom(const char *filename) const;
};

#endif