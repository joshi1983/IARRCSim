#ifndef HTTPREQUEST_HEADER
#define HTTPREQUEST_HEADER

#include "Socket.hpp"
#include "HTTPParser.hpp"
#include "ParameterCollection.hpp"

#include <list>
#include <iostream>
#include <string>

class HTTPRequest
{
 private:
   HTTPFieldSet fields;  
   ParameterCollection gets;
   ParameterCollection posts;

   // mainly from POST requests since GET requests have no body.
   char *content;
   int contentLength;

   std::string requestPath;
   std::string filename; 
    // derived from requestPath

   bool postsUpdated;

   void readFrom(Socket &s);

   // should be called whenever the requestPath changes.
   void updateFileName(); 
   void updateGetParameters();
   void updatePostParameters();

 public:
   // for reading in a request from the specified socket
   HTTPRequest(Socket &s);
   
   void writeTo(std::ostream &out) const;
   const char * getGET(const char *name) const;
   const char * getPOST(const char *name);
   const char * getParam(const char *name);
   const std::string getHost() const;
   const std::list<Parameter>& getGETParameters() const;
   const std::string & getRequestPath() const;
   const std::string & getRequestFileNamePath() const;
   const char * getContent() const;
   int getContentLength() const;

   /**
   Checks out of a set of MIME type strings, 
   which one is most preferred by the browser that sent this request.
This is used for content type negotiation.
   */
   int getPreferredMIMEType(std::string *MIMEtypeArray,int len) const;
};

#endif
