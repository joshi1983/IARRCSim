#ifndef HTTPPARSER_HEADER
#define HTTPPARSER_HEADER

#include "../stdafx.h"
#include "HTTPFieldSet.hpp"
#include "Socket.hpp"

class HTTPParser
{
 public:
	 static void load(Socket &s,HTTPFieldSet &fields,
		 std::string &requestPath,char **content,int *contentLength);
	 static void load(std::string &s,HTTPFieldSet &fields,
		 std::string &requestPath);


	 static void urlDecode(std::string & s);
};

#ifndef _UBUNTU_

void convertToWideString(WCHAR *dest,char *s);

#endif

std::string asString(double val);
double asDouble(std::string val);
void trimString(std::string &str);
bool stringEndsWith(const std::string&s1,const char *ending);
bool stringStartsWith(const std::string&s1,const char *starting);
bool stringStartsAt(const std::string &s,const char *subs,int index);
std::string getAbsolutePath(const std::string &filename,const std::string &curFilePath);

#endif