#include "HTTPResponse.hpp"
#include "HTTPParser.hpp"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

const char RESPONSE_CODE_FILENAME[]="templates/HTTPResponses.txt";

struct ResponseCode
{
 const int code;
 const string message;

 ResponseCode(int code,char * message): 
   code(code),message(message)
 {}

 ResponseCode(int code,const string& message): 
   code(code),message(message)
 {}

   // copy constructor
   ResponseCode(const ResponseCode &rc): code(rc.code),message(rc.message)
   {
   }
};

static list<ResponseCode> codes;

static string defaultResponseMessage("OK");

void HTTPResponse::loadHTTPResponseCodes()
{
 codes.clear();

 ifstream in;
 in.open(RESPONSE_CODE_FILENAME);

 if (!in)
 {
   cout << "Unable to open file: '"
	   <<RESPONSE_CODE_FILENAME<<"'"<<endl;
   cout << "Did you extract the files before running?"<<endl;
   cout << "You must extract all files and run the simulator from that folder."
	   <<endl;
   return;
 }

 string line;
 int code;
 string message;
 string::size_type pos;

 // loop through lines of the file.
 while (getline(in,line))
 {
    if (line.length()<3)
	    continue;

    pos = line.find(' ');

    if (pos==string::npos)
    {
	    cout << RESPONSE_CODE_FILENAME
		    <<": required space missing in line: "
		    <<line<<endl;
	    continue;
    }

    string codeStr=line.substr(0,pos);
    // read the character code.
    code=atoi(codeStr.c_str());

    // get the message.
    message=line.substr(pos+1);
    trimString(message);

    // trim the preceding spaces.
    codes.push_back(ResponseCode(code,message));
 }

 in.close();
}

const string & HTTPResponse::getMessageForResponseCode(int code)
{
	list<ResponseCode>::const_iterator it = codes.begin();

	for (;it!=codes.end();it++)
	{
	   if (it->code==code)
	   {
	      return it->message;
	   }
	}

	return defaultResponseMessage;
}

HTTPResponse::HTTPResponse():responseCode(200),content(NULL),contentLength(0)
{
	setContent(NULL,0);
	setHeader("Server","IARRC Simulator");
}

HTTPResponse::HTTPResponse(int responseCode): 
 responseCode(responseCode),content(NULL),contentLength(0)
{
	setContent(NULL,0);
	setHeader("Server","IARRC Simulator");
}

void HTTPResponse::setContent(const char *content,int len)
{
  if (this->content!=NULL)
	delete []this->content;

 char lenStr[100];

  this->contentLength=len;
  sprintf(lenStr,"%d",contentLength);
  fields.add(string("Content-Length"),string(lenStr));
  this->content=content;
}

void HTTPResponse::setHeader(const char *name,const char *value)
{
  setHeader(string(name),string(value));
}

void HTTPResponse::setHeader(const string &fieldName,const string &value)
{
  fields.add(fieldName,value);
}

void HTTPResponse::setHTTPResponseCode(int responseCode)
{
  this->responseCode=responseCode;
}

void HTTPResponse::writeTo(Socket &s) const
{
 char firstLine[200];

  sprintf(firstLine,"HTTP/1.0 %d %s\n",responseCode,
	  HTTPResponse::getMessageForResponseCode(responseCode).c_str());

  s.write(firstLine,strlen(firstLine));

  fields.writeTo(s);

  s.write("\n",1); 
   // mark end to HTTP head and beginning to body.

  if (content!=NULL)
     s.write(content,contentLength);
}

void HTTPResponse::writeTo(std::ostream &out) const
{
 char firstLine[200];
 ostringstream oss;

  sprintf(firstLine,"HTTP/1.1 %d %s\n",responseCode,
	  HTTPResponse::getMessageForResponseCode(responseCode).c_str());

  oss << firstLine;

  fields.writeTo(oss);

  oss << "\r\n"; 
   // mark end to HTTP head and beginning to body.

  if (content!=NULL)
  {
	for (int i=0;i<contentLength;i++)
         oss << content[i];
  }
  out << oss.str();
}

const char *HTTPResponse::getRecommendedMimeFor(const std::string &filename)
{
  if (stringEndsWith(filename,".html")||stringEndsWith(filename,".htm")||
	  stringEndsWith(filename,".thtml"))
  {
    return "text/html";
  }
  else if (stringEndsWith(filename,".txt"))
  {
    return "text/plain";
  }
  else if (stringEndsWith(filename,".png"))
  {
    return "image/png";
  }
  else if (stringEndsWith(filename,".jpg")||stringEndsWith(filename,".jpeg"))
  {
    return "image/jpeg";
  }
  else if (stringEndsWith(filename,".ico"))
  {
    return "image/vnd.microsoft.icon";
  }
  else if (stringEndsWith(filename,".gif"))
  {
    return "image/gif";
  }
  else if (stringEndsWith(filename,".js"))
  {
    return "text/javascript";
  }
  else if (stringEndsWith(filename,".css"))
  {
    return "text/css";
  }
  else if (stringEndsWith(filename,".xml"))
  {
      return "text/xml";
  }
  
  return "text/html";
}

