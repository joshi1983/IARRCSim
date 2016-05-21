#include "Utilities.hpp"

#include "../stdafx.h"
#include "../docserver/DocServer.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef _UBUNTU_

#include <windows.h>
#include <cstdio>

#endif

void Utilities::launchRelativeDocument(const char *relPath)
{
  char s[300];

  sprintf(s,"http://localhost:%d/%s",DocServer::getDocServerPort(),relPath);

  launchURL(s);
}

void Utilities::launchURL(const char *url)
{
 #ifndef _UBUNTU_

 static
 #ifdef _USE_VISUAL_STUDIO_
  wchar_t
 #else
 char
 #endif
  wText1[5] = {'o','p','e','n','\0'};
 int len=strlen(url);

 #ifdef _USE_VISUAL_STUDIO_
  wchar_t
 #else
 char
 #endif
 *wURL = new
 #ifdef _USE_VISUAL_STUDIO_
  wchar_t
 #else
 char
 #endif
 [len+1];
   for (int i=0;url[i]!='\0';i++)
        wURL[i]=url[i];

   wURL[len]='\0';

   ShellExecute(NULL, wText1, wURL,
    NULL, NULL, SW_SHOWNORMAL);

  delete []wURL;

 #else

 char s[300];

   sprintf(s,"xdg-open %s",url);

   system(s);

 #endif

}
