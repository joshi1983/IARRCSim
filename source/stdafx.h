#ifndef STDAFX_HEADER
#define STDAFX_HEADER

#define PI 3.14159265358979

#include "io/Socket.hpp"

#ifndef _USE_VISUAL_STUDIO_



#ifdef _UBUNTU_
#include <GL/freeglut.h>
#include <GL/glut.h>

typedef char BYTE;

typedef short int WORD;
typedef int DWORD;

#else  // in windows or Mac but not using Visual Studio

#ifdef __APPLE__
#include <GLUT/glut.h>
#else // using mingw on Windows

#include <GL/freeglut.h>
#endif // end if apple else windows

#endif // end if ubuntu or not

#endif // end if not USE_VISUAL_STUDIO

#ifdef _UBUNTU_

#   define FILE_SEPERATOR '/'

#else

#   define FILE_SEPERATOR '\\'

#endif

#endif
