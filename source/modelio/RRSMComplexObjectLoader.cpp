#include "../stdafx.h"
#include "RRSMComplexObjectLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>

#include "../app/Application.hpp"
#include "../physicalobjects/Quad.hpp"
#include "../physicalobjects/UseTextureCommand.hpp"
#include "../physicalobjects/TranslateObjectWrapper.hpp"
#include "../physicalobjects/PolygonCommand.hpp"

using namespace std;

struct TextureEntry
{
  string filename;
  Texture *txt;

  TextureEntry(const char *filename,Texture *txt):
    filename(filename),txt(txt)
  {
  }
};

struct ComplexObjectEntry
{
string filename;
  ComplexObject *co;

  ComplexObjectEntry(const char *filename,ComplexObject *co):
    filename(filename),co(co)
  {
  }
};

list<TextureEntry> textureCache;
list<ComplexObjectEntry> complexObjectCache;

Texture * getTextureFromCache(list<TextureEntry> & l,const char *filename)
{
	list<TextureEntry>::iterator it = l.begin();

	for (;it!=l.end();it++)
	{
	   if ((*it).filename==filename)
	   {
	      return it->txt;
	   }
	}

	return NULL;
}

ComplexObject * getComplexObjectFromCache(list<ComplexObjectEntry> & l,const char *filename)
{
	list<ComplexObjectEntry>::iterator it = l.begin();

	for (;it!=l.end();it++)
	{
	   if ((*it).filename==filename)
	   {
	      return it->co;
	   }
	}

	return NULL;
}

string getParameter(const string &line,const char *funcName)
{
    // line = "loadtexture bla.bmp  func(234)  transparent..."
	string::size_type pos = line.find(funcName);
	if (pos==string::npos)
		return string("");

	string s=line.substr(pos+strlen(funcName));
	// s = "(234)  transparent..."
	pos = s.find(')');
	if (pos==string::npos)
		return string("");

	s=s.substr(0,pos);
	// s = "(234"
	pos=s.find('(');
	if (pos==string::npos)
	   return string("");

	s=s.substr(pos+1);
     // s = "234"

	// let's remove trailing and preceding whitespaces.


	return s;
}

/**
 Uses an absolute path and a relative path to resolve another absolute path.
 For example, resolveAbsolutePath("models/traxxas/car.rssm","wheel.rssm",result)
 This example will store "models/traxxas/wheel.rssm" into the result variable.
*/
void resolveAbsolutePath(const char *path,const char *relPath,char *result)
{
 int i;

  for (i=strlen(path)-1;i>=0;i--)
  {
     if (path[i]=='/' || path[i]=='\\')
	{
	   break;
	}
  }

  strncpy(result,path,i+1);
  // copy the "models/traxxas/" part of it.

  strcpy(result+i+1,relPath);
  // copy the "wheel.rssm" part of it.

  // now result should be ie. "models/traxxas/wheel.rssm"
}

ComplexObject* RRSMComplexObjectLoader::loadFrom(const char *filename) const
{
 ComplexObject * result = new ComplexObject();

 ifstream ifs;

 char filenameBuf[300];
 strcpy(filenameBuf,filename);
 fixFilenameSlashes( filenameBuf);

 ifs.open(filenameBuf);

 if (!ifs)
 {
	 cout << "Unable to load file: "<<filenameBuf<<endl;
	 delete result;
	 return NULL;
 }


 string line;

 // loop through lines of the file.
 while (getline(ifs,line))
 {
   if (line.length()<3)
	 continue;
   if (line[0]=='#')
	   continue;

   istringstream iss(line,ios_base::in);
   string commandName;

   iss >> commandName;

   if (commandName=="loadcolour")
   {
      double r,g,b;

	 iss>>r;
	 iss>>g;
      iss>>b;

	   // create instance of UseTextureCommand.
      UseTextureCommand * useTextureCommand =
		 new UseTextureCommand(r,g,b);

	 result->add(useTextureCommand);
   }
   else if (commandName=="loadtexture")
   {
	 char fn[200];
	 string fns;
	 iss >> fns;

	 resolveAbsolutePath(filenameBuf,fns.c_str(),fn);
	 // get absolute file path.
      // create instance of Texture.
	 Texture *txt=NULL;
	  txt = getTextureFromCache(textureCache,fn);
      bool transparent = false;

	 if (line.find("transparent-on")!=string::npos ||
		 line.find("transparent-colour")!=string::npos)
	  {
	    transparent=true;
	  }

	  if (txt==NULL)
	  {
		string::size_type pos = line.find("transparent-colour");
		if (pos!=string::npos)
		{
		   string s=line.substr(pos);
		   // s is for example "transparent-colour(220,220,220)"
		   string::size_type i = s.find('(');

		   if (i!=string::npos)
		   { // i is index of '('.
		      // let's get the comma separated integers.
                char s2[100];
			 strncpy(s2,s.c_str()+i+1,99);
			 char *rTok=strtok(s2,", \t\r\n)");
			 if (rTok!=NULL)
			 {
				 char *gTok=strtok(NULL,", \t\r\n)");
				 if (gTok!=NULL)
				 {
					 char *bTok=strtok(NULL,", \t\r\n)");
					 if (bTok!=NULL)
					 {
						int r,g,b;
						r=atoi(rTok);
						g=atoi(gTok);
						b=atoi(bTok);
                              txt = new Texture(fn,r,g,b);
					 }
				 }
			 }
		   }
		   if (txt==NULL)
		   {
		     cout << "Invalid transparent-colour syntax in: "<<line<<endl;
		   }
		}
	     else
			txt = new Texture(fn,transparent);

		if (txt!=NULL)
		    textureCache.push_back(TextureEntry(fn,txt));
	  }

      // create instance of UseTextureCommand.
      UseTextureCommand * useTextureCommand =
		 new UseTextureCommand(txt);

	 result->add(useTextureCommand);
   }
   else if (commandName=="load")
   {
	char fn[200];
     string fns;
	iss >> fns;

	resolveAbsolutePath(filenameBuf,fns.c_str(),fn);

     ComplexObject * o=NULL;
	o=getComplexObjectFromCache(complexObjectCache,fn);
	if (o==NULL)
	{
  	     o=loadFrom(fn);
          complexObjectCache.push_back(ComplexObjectEntry(fn,o));
	}

	Vector3D translation;

	translation.readFrom(iss);

	// wrap it up in an object that will translate it.
     TranslateObjectWrapper *wrapper = new TranslateObjectWrapper(translation,o);

     // check for rotation flags.
	// check for "flip-x"
	if (line.find("high-res-only")!=string::npos)
	{
         wrapper->setForHighRes(true);
	}
	string name = getParameter(line,"name");
	if (name.c_str()[0]!='\0')
	{
	     wrapper->setName(name);
	}
	string rotVal=getParameter(line,"x-rotate");

	rotVal = getParameter(line,"x-rotate");
	if (rotVal.c_str()[0]!='\0')
	{
		wrapper->setXRotation(atoi(rotVal.c_str()));
	}
	rotVal = getParameter(line,"y-rotate");
	if (rotVal.c_str()[0]!='\0')
	{
		wrapper->setYRotation(atoi(rotVal.c_str()));
	}
     rotVal = getParameter(line,"z-rotate");
	if (rotVal.c_str()[0]!='\0')
	{
		wrapper->setZRotation(atoi(rotVal.c_str()));
	}

	result->add(wrapper);
   }
   else if (commandName=="quad")
   {
	Vector3D points[4];

     // loop through the 4 points.
     for (int i=0;i<4;i++)
	{
	   points[i].readFrom(iss);
	}

	Quad *q = new Quad(points[0],points[1],points[2],points[3]);

	string arg;
	if (iss)
	{
	   iss>>arg;
	   if (arg=="-normal")
		   q->negateNormal();
	}
	 result->add(q);
   }
   else if (commandName=="circle")
   {
     int numSides;
	Vector3D offset;
     double radius;
     PolygonCommand *poly=new PolygonCommand();

	offset.readFrom(iss);

	iss >> radius;
	iss >> numSides;

     for (int side=0;side<numSides;side++)
	{
	   double angle1=side*PI*2/numSides;
        Vector3D v(offset.getX(),offset.getY()+radius*cos(angle1),
		   offset.getZ()+radius*sin(angle1));

	   poly->add(v);
	}

	poly->updateRanges();
	result->add(poly);
   }
   else if (commandName=="poly")
   {
	PolygonCommand *poly=new PolygonCommand();
     int numPoints;
	iss>>numPoints;

	for (int i=0;i<numPoints;i++)
	{
	 Vector3D v;

         v.readFrom(iss);
	    poly->add(v);
	}

	result->add(poly);
   }
   else if (commandName=="cylinder")
   {
     int numSides;
     Vector3D offset;

	offset.readFrom(iss);

     double radius;
	double length; // how long is the cylinder?

	iss >> radius;
	iss >> length;
	iss >> numSides;

     // loop through sides.
     for (int side=0;side<numSides;side++)
	{
	   double angle1=side*PI*2/numSides;
        double angle2=(side+1)*PI*2/numSides;

	   Vector3D p[4];
        // calculate point positions.

        double z,y;

	   y=offset.getY()+radius*cos(angle1);
	   z=offset.getZ()+radius*sin(angle1);

	   p[0].set(offset.getX()-length/2,y,z);
        p[1].set(offset.getX()+length/2,y,z);

	   y=offset.getY()+radius*cos(angle2);
	   z=offset.getZ()+radius*sin(angle2);

	   p[2].set(offset.getX()+length/2,y,z);
        p[3].set(offset.getX()-length/2,y,z);

        Quad *q=new Quad(p[0],p[1],p[2],p[3]);

	     q->negateNormal();

	     // add new quad for it.
	     result->add(q);
	}
   }
   else if (commandName=="geo")
   {
      // load geographical positioning information.
    double latitudeDegrees;
    double longitudeDegrees;
    double rotationAngle;

      iss>>latitudeDegrees;
	 iss>>longitudeDegrees;
	 iss>>rotationAngle;

	 result->setGeoOriginAndOrientation(latitudeDegrees,longitudeDegrees,rotationAngle);
   }
   else if (commandName=="boundrect")
   {
     double xRange,zRange;
       iss>>xRange;
       iss>>zRange;

       result->setBoundaries(xRange,zRange);
       //cout << "RRSMComplexObjectLoader setting boundaries to "<<xRange<<" "<<zRange<<endl;
   }
   else
   {
	   cout << "Unrecognized command: '"<<commandName<<"'"<<endl;
	  // delete result;
	  // return NULL;
   }
 }

  return result;

}
