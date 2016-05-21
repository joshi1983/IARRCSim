#include "HTTPRequest.hpp"

using namespace std;

const std::list<Parameter>& HTTPRequest::getGETParameters() const
{
  return gets.getParameters();
}

const char * HTTPRequest::getContent() const
{
  return content;
}

int HTTPRequest::getContentLength() const
{
  return contentLength;
}

void HTTPRequest::readFrom(Socket &s)
{
  // use HTTPParser to load HTTPFieldSet.

	HTTPParser::load(s,fields,requestPath,&content,&contentLength);
	updateFileName();
	updateGetParameters();
}

HTTPRequest::HTTPRequest(Socket &s): postsUpdated(false),content(NULL),contentLength(0)
{
  readFrom(s);
  updateFileName();
  updateGetParameters();
}

void HTTPRequest::updateFileName()
{
 string::size_type pos = requestPath.find('?');
 if (pos==string::npos)
 {
   filename=requestPath;
 }
 else
 {
    filename=requestPath.substr(0,pos);
 }
}

const std::string & HTTPRequest::getRequestFileNamePath() const
{
  return filename;
}

int HTTPRequest::getPreferredMIMEType(string *MIMEtypeArray,int len) const
{
 const string *val=fields.getValueFor(string("Accept"));

  if (val==NULL)
     return 0;

  // see if any of the mime types are actually found in val.
  for (int i=0;i<len;i++)
  {
	  if (val->find(MIMEtypeArray[i])!=string::npos)
	  { // perfect match
	     return i;
	  }
  }

  return 0;
}

void HTTPRequest::writeTo(ostream &out) const
{
  out << "GET "<<requestPath<<" HTTP/1.0"<<endl;
  fields.writeTo(out);
}


void extractURLEncodedParameters(const string &s,ParameterCollection & result)
{
 string remaining=s;

  while (1)
  {
    string::size_type pos=remaining.find('=');
    if (pos==string::npos)
	    break;

    string name=remaining.substr(0,pos);
    string value;
    string::size_type pos2=remaining.find('&');
    if (pos2==string::npos) // last value
    {
      value=remaining.substr(pos+1);
    }
    else
    {
       value=remaining.substr(pos+1,pos2-pos-1);
    }
    HTTPParser::urlDecode(value);

    result.add(name.c_str(),value.c_str());
    if (pos2==string::npos)
	    break;
    else
	    remaining=remaining.substr(pos2+1);
  }
}

void HTTPRequest::updatePostParameters()
{
  // if already updated, don't go through the long process again.
  if (postsUpdated) 
	  return;

  if (content!=NULL) // there is some content
  {
     string contents(content,contentLength);
      extractURLEncodedParameters(contents,posts);
  }

  postsUpdated=true;
}


void HTTPRequest::updateGetParameters()
{
 string::size_type pos = requestPath.find('?');
  gets.clear();
  if (pos==string::npos)
	 return;

 string remaining=requestPath.substr(pos+1);

  extractURLEncodedParameters(remaining,gets);
}

const std::string & HTTPRequest::getRequestPath() const
{
  return requestPath;
}

const char * HTTPRequest::getGET(const char *name) const
{
  return gets.getAsCharPtr(name);
}

const std::string HTTPRequest::getHost() const
{
 std::string result("localhost");
 
  const std::string *host=fields.getValueFor(string("Host"));

  if (host!=NULL)
  {
	  result=*host;
	  // eliminate the port number if it is there.
	  string::size_type i = result.find(":");
	  if (i!=string::npos)
	  {
	     result = result.substr(0,i);
	  }
  }

  return result;
}

const char * HTTPRequest::getPOST(const char *name)
{
    updatePostParameters();

    return posts.getAsCharPtr(name);
}

const char * HTTPRequest::getParam(const char *name)
{
  const char * result = getGET(name);

  if (result!=NULL)
	  return result;
  else
	  return getPOST(name);
}


