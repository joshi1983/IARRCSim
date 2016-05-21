#include "LogMessage.hpp"
#include <sstream>

using namespace std;

LogMessage::LogMessage(double time,const std::string& description):time(time),description(description)
{

}

/**
 Returns a description of this message in the specified format.
The format can be txt, JSON, or RSS.
*/
std::string LogMessage::toString(const std::string &format)
{
 ostringstream result;

  if (format=="txt")
     result<<time<<": "<<description<<endl;
  else if (format=="xml")
  {
     result<<"<LogMessage time=\""<<time<<"\">"<<endl;
     result<<description<<endl;
     result<<"</LogMessage>";
  }
  else if (format=="JSON")
  {
     result<<"{\"time_ms\":"<<time<<",\"description\":\""<<description<<"\"}"<<endl;
  }
  else if (format=="RSS")
  {
     // some message for RSS?

  }
  

  return result.str();
}
