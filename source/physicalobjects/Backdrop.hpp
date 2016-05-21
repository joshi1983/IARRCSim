#ifndef BACKDROP_HEADER
#define BACKDROP_HEADER

#include "ComplexObject.hpp"
#include <string>

/**
Represents 3D backgrounds or scenery.
*/
class Backdrop
{
 private:
   std::string id;
   std::string filename;   
   std::string imageURL;
   bool active;

 public:
   Backdrop(const std::string &id,const std::string &filename,const std::string &imageURL);

   /**
    Set to true whenevever this backdrop is selected for the environment
   */
   void setActive(bool active);

   std::string getID() const;

   std::string getImageURL() const;

   /**
    Loads from internal filename
   */
   ComplexObject * createComplexObject();

   /**
    Represents this object in various formats
   */
   std::string getFormatted(const std::string &format);

   /**
    Returns the associated model file name
   */
   std::string getFilename() const;
};


#endif