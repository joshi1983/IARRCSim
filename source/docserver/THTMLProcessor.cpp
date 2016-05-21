#include "THTMLProcessor.hpp"

#include "SingleVariableValue.hpp"
#include "ArrayVariableValue.hpp"
#include "Variable.hpp"

#include "../io/HTTPParser.hpp"

#include <cstdlib>
#include <cstdio>
#include <sstream>

using namespace std;


string::size_type getCounterBalancedEnding(const string& s,int start)
{
  static const char *Ending="-->";
  static const char *Beginning="<!--#";
  int balance=1;

  // loop through all characters.
  for (unsigned int i=start;i<s.length();i++)
  {
    if (s[i]!='-' && s[i]!='<')
	    continue;

    // are we at the start of Ending?
    // are we at the start of Beginning?
    if (stringStartsAt(s,Beginning,i))
    {
      balance++;
    }
    else if (stringStartsAt(s,Ending,i))
    {
      balance--;
    }
    if (balance==0)
    {
      return (string::size_type)i;
    }
  }
  return string::npos; // indicate not found.
}


bool THTMLProcessor::getProcessedFileContents(const string&filename,VariableCollectionStack &variables,vector<char>&result)
{
  vector<char> contents;
  FILE *fp=fopen(filename.c_str(),"rb");

  if (fp!=NULL)
  {
	  char buf[200];
	  int count=0;

	  // read the file contents into a string object.
       while (!feof(fp))
	  {
	    int numRead = fread(&buf,1,200,fp);
         if (numRead<0)
		    break;

	    if (numRead>0)
	    {
		    count+=numRead;

		 for (int i=0;i<numRead;i++)
		 {
	        contents.push_back(buf[i]);
		 }
	    }
	  }

	  fclose(fp);

	  // if the file ends with ".thtml", process it as THTML.
	  if (stringEndsWith(filename,".thtml"))
	  {
	     // use the same scope like PHP does.
		  THTMLProcessor::processTHTMLDoc(filename,contents,variables);
	  }
       result=contents;
	  return true;
  }
  else
	  return false;

}

bool THTMLProcessor::getProcessedFileContents(const string&filename,VariableCollectionStack &variables,string&result)
{
  vector<char> r;
  bool retVal=getProcessedFileContents(filename,variables,r);
  char *buf=(char*)malloc(r.size());

  for (unsigned int i=0;i<r.size();i++)
	  buf[i]=r[i];

  result=string(buf,r.size());

  return retVal;
}



void THTMLProcessor::processTHTMLDoc(const string &filepath,string &contents,VariableCollectionStack &variables)
{
  // look for "<!--#" in contents.
  // process each one of them.

	string result;
	string remaining=contents;
	string::size_type pos;

	while (1)
	{
  	    pos=remaining.find("<!--#");
	    if (pos==string::npos)
	    {
		    result+=remaining;
		    break;
	    }

	    result+=remaining.substr(0,pos);
	    remaining=remaining.substr(pos+5);
	    // remaining = "set ...--> ..."

	    // what is the command?
	    string::size_type pos2=remaining.find(' ');

	    if (pos2==string::npos)
	    {
	       cout << "missing space in THTML command." << endl;
		  result+=remaining;
		  break;
	    }
	    string command=remaining.substr(0,pos2);
	    remaining=remaining.substr(pos2+1);
	    if (command=="host")
	    { // no parameters.
		    // just print the host name.
	       string::size_type pos3=remaining.find("-->");
		 if (pos3==string::npos)
		 {
		    cout << "THTML command error. A(n) "<< command
			    <<" command is missing its closing -->"<<endl;

		    break;
		 }
		 remaining=remaining.substr(pos3+3);

		    string val = variables.getValueAsStringFor(
			    string("headers.Host"));

            if (val=="")
               result+="localhost";
            else
		     result+=val;
	    }
	    else if (command=="set" || command=="include" || command=="echo"
		    || command=="array-add")
	    { // 2 paramaeters
		 string::size_type pos3=remaining.find("-->");
		 if (pos3==string::npos)
		 {
		    cout << "THTML command error. A(n) "<< command
			    <<" command is missing its closing -->"<<endl;

		    break;
		 }
           string inText=remaining.substr(0,pos3);
		 istringstream iss(inText);
           string param1;

		  iss>>param1;

		  remaining=remaining.substr(pos3+3);
	      if (command=="echo")
		 {
			 result+=variables.getValueAsStringFor(param1);
		 }
		 else if (command=="include")
		 {
		   string resultingContents;
		   variables.pushEmptyScope();
		   getProcessedFileContents(getAbsolutePath(param1,filepath)
			   ,variables,resultingContents);

			variables.popScope();

		     result+=resultingContents;
		 }
		 else if (command=="set" || command=="array-add")
		 { // 2 parameters
		    string param2;
		    getline(iss,param2);
		    trimString(param2);

		    SingleVariableValue *val=new SingleVariableValue(param2);

              if (command=="array-add")
		    {
                 variables.arrayAdd(param1,param2);
		    }
		    else // set.
		    {
		       variables.setOrDeclare(param1,val);
		    }
		 }
	    }
	    else if (command=="for-each")
	    {
		    string::size_type pos2=getCounterBalancedEnding(remaining,0);
		    if (pos2==string::npos)
		    {
		        cout << "unbalanced THTML for-each command."<<endl;
			   break;
		    }

              string section=remaining.substr(0,pos2);
              pos=section.find('\n');
		    string arrayName;
		    string varName;
		    if (pos!=string::npos)
		    {
                string line = section.substr(0,pos);
			 istringstream iss(line);
			  iss>>arrayName;
			  iss>>varName;

		       section=section.substr(pos);
		    }
		    const VariableValue *v=variables.getVariableValueByName(arrayName);

              const ArrayVariableValue* av=NULL;

		    if (v!=NULL)
		       av= dynamic_cast<const ArrayVariableValue*>(v);

              if (av==NULL)
		    {
		       cout << "Variable '"<<arrayName<<"' is not an array."<<endl;
		    }
		    else
		    {
		      int sz=av->size();
			  // loop through the array type variable.
			 for (int i=0;i<sz;i++)
			 {
			  string resultingContents;
		       variables.pushEmptyScope();
			  Variable *v=new Variable(varName,(*av)[i]->getClone());
			  variables.declareNamedValue(v);
			   processTHTMLDoc(filepath,section,resultingContents,variables);
			  variables.popScope();

		        result+=resultingContents;
			 }
		    }

		    remaining=remaining.substr(pos2+3);
	    }
	    else
	    {
		    cout << "Unrecognized THTML command: "<< command<<endl;
	    }
	}
    contents=result;
}

void THTMLProcessor::processTHTMLDoc(const string &filepath
				 ,const string &contents
				 ,string &resultingContents
				 ,VariableCollectionStack& variables)
{
 string contentBuf(contents);
  processTHTMLDoc(filepath,contentBuf,variables);
  resultingContents=contentBuf;
}

void THTMLProcessor::processTHTMLDoc(const string &filepath,string &contents)
{
 VariableCollectionStack stack;

   processTHTMLDoc(filepath,contents,stack);
}


void THTMLProcessor::processTHTMLDoc(const string &filepath,vector<char> &contents,VariableCollectionStack& variables)
{
  char *sPtr=(char*)malloc(contents.size()+1);
  for (unsigned int i=0;i<contents.size();i++)
      sPtr[i]=contents[i];

  string s(sPtr,contents.size());

   processTHTMLDoc(filepath,s,variables);

	contents.clear();
	for (unsigned int i=0;i<s.length();i++)
		contents.push_back(s[i]);
}

void THTMLProcessor::processTHTMLDoc(const string &filepath,vector<char> &contents)
{
 char *s=(char*)malloc(contents.size());
 for (unsigned int i=0;i<contents.size();i++)
	 s[i]=contents[i];

 string str(s,contents.size());

 processTHTMLDoc(filepath,str);
 // copy it back into the vector.
 contents.clear();
 for (unsigned int i=0;i<str.length();i++)
	 contents.push_back(str[i]);
}
