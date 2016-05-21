#ifndef HTTPRESPONSE_HEADER
#define HTTPRESPONSE_HEADER

#include "HTTPFieldSet.hpp"
#include "Socket.hpp"
#include <string>


class HTTPResponse
{
 private:
   HTTPFieldSet fields;
   int responseCode;
   const char *content;
   int contentLength;

   static const std::string & getMessageForResponseCode(int code);

 public:
   HTTPResponse();
   HTTPResponse(int responseCode);

   void setHTTPResponseCode(int responseCode);
   void setContent(const char *content,int len);
   void setHeader(const char *name,const char *value);
   void setHeader(const std::string &fieldName,const std::string &value);
   void writeTo(Socket &s) const;
   void writeTo(std::ostream &out) const;

   static void loadHTTPResponseCodes();
   static const char *getRecommendedMimeFor(const std::string &filename);
};

#endif