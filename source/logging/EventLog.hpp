#ifndef EVENT_LOG_HEADER
#define EVENT_LOG_HEADER

#include <list>
#include "LogMessage.hpp"

class EventLog
{
 private:
   std::list<LogMessage> messages;

 public:
   EventLog();
   std::string toString(const std::string & format);

};

#endif