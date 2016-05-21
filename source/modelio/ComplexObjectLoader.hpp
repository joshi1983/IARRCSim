#ifndef MODEL_LOADER_HEADER
#define MODEL_LOADER_HEADER

#include "../physicalobjects/ComplexObject.hpp"

class ComplexObjectLoader
{

public:
   virtual ComplexObject* loadFrom(const char *filename) const=0;
};

#endif