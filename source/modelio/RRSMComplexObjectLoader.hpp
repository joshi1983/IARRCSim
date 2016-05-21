#ifndef RRSM_COMPLEX_OBJECT_LOADER
#define RRSM_COMPLEX_OBJECT_LOADER

#include "ComplexObjectLoader.hpp"

class RRSMComplexObjectLoader: public ComplexObjectLoader
{

 public:
   virtual ComplexObject* loadFrom(const char *filename) const;
};

#endif