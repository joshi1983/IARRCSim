#ifndef LOG_MESSAGE_HEADER
#define LOG_MESSAGE_HEADER
#include <string>

class LogMessage
{
 private:
   // when was this logged?
   double time;

   std::string description;

 public:
   LogMessage(double time,const std::string& description);

   /**
     Returns a description of this message in the specified format.
    The format can be txt, JSON, or RSS.
   */
   std::string toString(const std::string &format);
};


#endif