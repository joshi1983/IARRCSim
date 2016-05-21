#include "EventLog.hpp"
#include <sstream>

using namespace std;

EventLog::EventLog()
{

}

std::string EventLog::toString(const std::string & format)
{
 ostringstream result;
 
  // loop through the messages.
  for (list<LogMessage>::iterator it=messages.begin();it!=messages.end();it++)
  {
      result<<(*it).toString(format);
  }

  return result.str();
}
