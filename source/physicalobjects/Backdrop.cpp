#include "Backdrop.hpp"
#include "../modelio/RRSMComplexObjectLoader.hpp"
#include <sstream>

using namespace std;

Backdrop::Backdrop(const std::string &id,const std::string &filename,const std::string &imageURL):
 id(id),filename(filename),imageURL(imageURL),active(false)
{

}

string Backdrop::getID() const
{
  return id;
}

/**
  Set to true whenevever this backdrop is selected for the environment
*/
void Backdrop::setActive(bool active)
{
   this->active=active;
}

string Backdrop::getImageURL() const
{
  return imageURL;
}

/**
 Loads from internal filename
*/
ComplexObject * Backdrop::createComplexObject()
{
  RRSMComplexObjectLoader loader;

    return loader.loadFrom(filename.c_str());
}


/**
  Represents this object in various formats
*/
string Backdrop::getFormatted(const std::string &format)
{
  ostringstream result;
 
   if (format=="xml")
   {
      result<<"<backdrop id=\""<<id<<"\" image=\""<<imageURL
		 <<"\" active=\""<<active<<"\" />"<<endl;
   }
   else if (format=="JSON")
   {
	   result<<"{"<<endl<<"  \"id\": \""<<id<<"\",\"image\":\""<<imageURL
		   <<"\",\"active\":\""<<active<<"\""
		   <<endl<<"}"<<endl;
   }
   else if (format=="txt" || format=="csv")
   {
         result<<id<<","<<imageURL
		 <<","<<active<<endl;
   }

  return result.str();
}

std::string Backdrop::getFilename() const
{
  return filename;
}