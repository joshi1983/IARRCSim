#include "HTTPParser.hpp"

#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

#ifndef _UBUNTU_

void convertToWideString(WCHAR *dest,char *s)
{
 int i;

  for (i=0;s[i]!='\0';i++)
	  dest[i]=s[i];

  dest[i]='\0';
}

#endif

bool stringEndsWith(const string&s1,const char *ending)
{
 const char *cpointer=ending;

  for (int i=s1.length()-strlen(ending);cpointer[0]!='\0';i++,cpointer++)
  {
    if (s1[i]!=cpointer[0])
	    return false;
  }
  return true;
}

bool stringStartsWith(const std::string&s1,const char *starting)
{
  for (int i=0;starting[i]!='\0';i++)
  {
    if (starting[i]!=s1[i])
	    return false;
  }

  return true;
}

bool stringStartsAt(const std::string &s,const char *subs,int index)
{
  for (int i=0;subs[i]!='\0';i++)
  {
    if (subs[i]!=s[index+i])
    {
       return false;
    }
  }
  return true;
}

void trimString(string &str)
{
  size_t startpos = str.find_first_not_of(" \t\r\n");
  size_t endpos = str.find_last_not_of(" \t\r\n");

  if (( string::npos == startpos ) || ( string::npos == endpos))
  {
	        str ="";
  }
  else
      str = str.substr( startpos, endpos-startpos+1 );
}

void HTTPParser::load(Socket &s,HTTPFieldSet &fields,string &requestPath
				  ,char **content,int *contentLength)
{
 int bufLen=1500;
 char buf[1501];
 int numRead;
 string requestText="";
 bool headerOnly=false;
 char contentStr[1501];

  // look for Content-length field or GET method type.
  while (1)
  {
     numRead = s.read(buf,bufLen);
	if (numRead<=0)
	{
		(*contentLength)=0;
		break;
	}

	buf[numRead]='\0';
     // if buf starts with "GET"
	if (string(buf).find("GET")==0)
	{
	  headerOnly=true;
	}

     requestText+=buf;

	// if contains "\n\n" or "\n\r\n", break.
	{
	   char *ss=strstr(buf,"\n\n");
	    if (ss!=NULL)
	    {
		    strcpy(contentStr,ss+2);
		    break;
	    }

	    ss=strstr(buf,"\n\r\n");
	    if (ss!=NULL)
	    {
		  strcpy(contentStr,ss+3);
	       break;
	    }
	}
  }

  load(requestText,fields,requestPath);

  const string *contentLenStr = fields.getValueFor(string("Content-length"));
  if (contentLenStr!=NULL)
  {
	int val;

	  sscanf(contentLenStr->c_str(),"%d",&val);
	  (*contentLength)=val;
	  (*content)=new char[val];
	  int len = strlen(contentStr);

	  // read the content from the socket.
	  if (val-len>0)
          s.read((*content)+len,val-len);

	  for (int i=0;i<len;i++)
	  {
	      (*content)[i]=contentStr[i];
	  }
  }
}

void HTTPParser::load(std::string &s,HTTPFieldSet &fields,std::string &requestPath)
{
 char *line;
 char *buf=(char*)malloc(sizeof(char)*(s.size()+1));
 char *name;
 char *value;

  strcpy(buf,s.c_str());

  line=strtok(buf,"\n");

  if (line==NULL) // no fields.
  {
    fields.clear();
    free(buf);
    return;
  }
  // look at first line to see if the requestPath can be extracted.
  if (line[1]!='T') // if not starting with "HTTP..."
  {
    char * pos = strchr(line,'/');
    if (pos==NULL)
  	  requestPath="";
    else
    {
      char *endChar = strchr(pos,' ');
      if (endChar!=NULL)
  	    (*endChar)='\0'; // terminate the NULL terminated string.

      requestPath=pos;
    }
  }
  else
	  requestPath="";

  fields.clear();

  // loop through lines.
  while (1)
  {
	// extract field name.
	name=strtok(NULL,":");
     if (name==NULL)
		break;

	value=strtok(NULL,"\n");

     // extract value.
     if (value==NULL)
		break;

	string valStr(value);
	trimString(valStr);
     fields.add(string(name),valStr);
  }

  free(buf);
}

/**
Converts the specified ASCII code of a hex digit into its numerical value.
*/
unsigned int hexdigitToInt(unsigned int i)
{
  if (i>='0' && i<='9')
	  return i-'0';
  else if (i>='a' && i<='z') // lower case letter.
	  return (i+10)-'a';
  else
	  return (i+10)-'A'; // upper case letter
}

/**
Converts 2 characters of a hex number into an ASCII character code
*/
unsigned int hexToInt(char c1,char c2)
{
  int v1=hexdigitToInt(c1);
  int v2=hexdigitToInt(c2);

  return (v1<<4)|(v2);
}

void HTTPParser::urlDecode(std::string & s)
{
 ostringstream result;

 // loop through characters.
 for (unsigned int i=0;i<s.length();i++)
 {
	 char c=s[i];

      if (c=='+')
	 {
	   result<<' ';
	 }
	 else if (c=='%' && i<s.length()-2)
	 { // convert character coding
	     int val = hexToInt(s[i+1],s[i+2]);
	   result<<(char)val;
	   i+=2;
	 }
	 else
	 {
	   result<<c;
	 }
 }

 s=result.str();
}

string asString(double val)
{
  ostringstream os;
  os<<val;
  return os.str();
}

/*
@param filename for example "hello.txt"
@param curFilePath for example "templates/public_html/doc.thtml"
@return for example "templates/public_html/hello.txt"
*/
string getAbsolutePath(const string &filename,const string &curFilePath)
{
 string cFilePath(curFilePath);
 string fname(filename);
 string::size_type pos = cFilePath.find_last_of('/');
 string::size_type pos1 = fname.find_last_of('/');

 if (pos==string::npos)
	 return fname;

 cFilePath=cFilePath.substr(0,pos);
 while (stringStartsWith(fname,"../"))
 {
    pos = cFilePath.find_last_of('/');
    if (pos!=string::npos)
       cFilePath=cFilePath.substr(0,pos);

    fname=fname.substr(3);
 }
 // ie. cFilePath = "templates/public_html"
 // ie. fname="hello.txt"

 return cFilePath+"/"+fname;
}

double asDouble(std::string val)
{
 istringstream iss(val);
 double result;

  iss >> result;

  return result;
}
