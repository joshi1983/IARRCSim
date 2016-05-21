#ifndef THTML_PROCESSOR_HEADER
#define THTML_PROCESSOR_HEADER

#include <string>
#include <vector>

#include "VariableCollectionStack.hpp"


class THTMLProcessor
{
 public:

  static void processTHTMLDoc(const std::string &filepath,std::string &contents);
  static void processTHTMLDoc(const std::string &filepath,std::vector<char> &contents);
  static void processTHTMLDoc(const std::string &filepath
				 ,const std::string &contents
				 ,std::string &resultingContents
				 ,VariableCollectionStack& variables);

  static void processTHTMLDoc(const std::string &filepath,
	  std::vector<char> &contents,VariableCollectionStack& variables);

 static void processTHTMLDoc(const std::string &filepath,std::string &contents,VariableCollectionStack &variables);

 static bool getProcessedFileContents(const std::string&filename,VariableCollectionStack &variables,
  std::string& result);

 static bool getProcessedFileContents(const std::string&filename,VariableCollectionStack &variables,
   std::vector<char>&result);

};

#endif