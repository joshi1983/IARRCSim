#include "FileDirectory.hpp"

#include "../stdafx.h"

#ifdef _UBUNTU_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#else

#include <windows.h>

#endif

#include <cstring>
#include <iostream>
#include <cstdio>


using namespace std;

/*
For Ubuntu version, check:
http://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/

*/

#ifdef _USE_VISUAL_CPP_

string asString(const wchar_t *vals)
{
  char path[200];
  int i=0;

  for (i=0;vals[i]!=0;i++)
  {
    path[i]=(char)vals[i];
  }
  path[i]='\0';

  return string(path,i);
}

#else

string asString(const char *vals)
{
  return string(vals);
}

#endif


std::list<std::string> FileDirectory::getFileNamesInDirectory(const std::string &dirName)
{
  list<string> result;

  //cout << "getFileNamesInDirectory called on "<<dirName<<endl;

  #ifdef _UBUNTU_
// for Ubuntu

    DIR *dp;
    struct dirent *dirp;
    if ( (dp  = opendir(dirName.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dirName << endl;
        return result;
    }

    while ((dirp = readdir(dp)) != NULL) {
        result.push_back(string(dirp->d_name));
    }
    closedir(dp);

#else

  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  #ifdef _USE_VISUAL_CPP_
  wchar_t
  #else
  char
  #endif
  DirSpec[MAX_PATH];
  unsigned int i;

   for (i=0;i<dirName.length();i++)
	   DirSpec[i]=dirName.c_str()[i];

   DirSpec[i]='/';
   DirSpec[i+1]='*';
   DirSpec[i+2]='\0';

   //cout << "getting all files within directory: "<<dirName<<endl;

   hFind = FindFirstFile(DirSpec, &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE)
	{
	   cout<<"Error: invalid path: "<<dirName<<endl;
           return result;
	}
	string temp=asString(FindFileData.cFileName);
	//cout << "got first file: "<<temp<<endl;
	if (temp!="." && temp!="..")
        result.push_back(temp);

	while(FindNextFile(hFind, &FindFileData) != 0)
	{
		temp=asString(FindFileData.cFileName);
		//cout << "got file: "<<temp<<endl;
	     if (temp!="." && temp!="..")
             result.push_back(temp);
	}

	FindClose(hFind);

#endif

  return result;
}

bool FileDirectory::fileExists(const char *filename)
{
  FILE *fp=fopen(filename,"r");
  if (fp==NULL)
	  return false;
  else
  {
     fclose(fp);
	return true;
  }
}
