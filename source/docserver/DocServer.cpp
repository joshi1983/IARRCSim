#include "DocServer.hpp"
#include "THTMLProcessor.hpp"
#include "Constant.hpp"

#include "../sensors/VideoDevice.hpp"
#include "../sensors/Lidar.hpp"

#include "../graphics/BitmapImage.hpp"

#include "../io/HTTPRequest.hpp"
#include "../io/HTTPResponse.hpp"
#include "../io/HTTPParser.hpp" // for some string functions

#include "../threads/Job.hpp"

#include "../app/Environment.hpp"
#include "../app/Application.hpp"

#include "../physicalobjects/Pylon.hpp"
#include "../physicalobjects/Ramp.hpp"
#include "../physicalobjects/TrafficLight.hpp"
#include "../physicalobjects/StopSign.hpp"
#include "../physicalobjects/VerticalWall.hpp"
#include "../modelio/RRSMComplexObjectLoader.hpp"

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace std;

int DocServer::port=2600;

struct DocRequest
{
  Socket *s;
  DocRequest(Socket *s):s(s)
  {
  }
  ~DocRequest()
  {
     delete s;
  }
};

class LidarScanJob: public Job
{
 private:
   Lidar *lidar;
   ostream &out;
   int numpoints;
   string format;

 protected:
    virtual void run()
	{
       lidar->writeDepthsTo(out,format);
	}

 public:
    LidarScanJob(ostream &out,Lidar *lidar,int numpoints,string format): out(out),lidar(lidar),numpoints(numpoints),format(format)
    {
    }
};

class BackdropLoaderJob: public Job
{
private:
	string id;

protected:

	virtual void run()
	{
	  Environment &e=Application::getMainEnvironment();
	    e.loadBackdropFrom(id);
	}

public:
	BackdropLoaderJob(const string &id): id(id)
	{}
};


class SwitchUIModeJob: public Job
{
private:
	int mode;
	bool bringtofront;

protected:

	virtual void run()
	{
	  SwitchToUI(mode);
	  if (bringtofront)
	  {
	    VideoDevice *cam = Application::getMainCamera();
	     if (cam!=NULL)
		    cam->bringToFront();
	  }
	}

public:
	SwitchUIModeJob(int mode,bool bringtofront):
	 mode(mode),bringtofront(bringtofront)
	{}
};

bool safeLoadBackdropFrom(const string &id)
{
   Application::SubmitJobForMainThread(new BackdropLoaderJob(id),true);

   return true;
}

void sendTextResponse(const char*text,DocRequest *r)
{
 HTTPResponse response;

  response.setHeader("Content-type","text/plain");
  string content(text);

  response.setContent(content.c_str(),content.length());
  response.writeTo(*(r->s));
}

void handleFileNotFound(const string &filename,DocRequest *r)
{
 ostringstream contentstream;
HTTPResponse response;

 contentstream<<"File not found: "<<filename<<endl;

 response.setHTTPResponseCode(404);
 response.setHeader("Content-type","text/plain");

 string content=contentstream.str();

 response.setContent(content.c_str(),content.length());
 response.writeTo(*(r->s));
}

void handleGETPathRequest(HTTPRequest & request,DocRequest *r)
{
string format="xml";

const char *formatPtr=request.getGET("format");

     if (formatPtr!=NULL)
     {
       format=formatPtr;
     }

	string content;
	const char *item = request.getGET("item");
	if (item==NULL)
	{
		sendTextResponse("item must be specified.",r);
		return;
	}
	else
	{
		bool sendContent=true;

		if (strcmp(item,"gps")==0)
		{
		  Environment &e = Application::getMainEnvironment();
		  Vector3D pos = Application::getMainCar()->getPosition();
		   content=e.getGeoPositionAsString(pos,format);
		}
		else if (strcmp(item,"backdrops")==0)
		{
			if (formatPtr==NULL)
				formatPtr="xml";

		    content=Application::getAllBackdropsFormatted(string(format));
		}
		else if (strcmp(item,"cars")==0)
		{
			Environment &e = Application::getMainEnvironment();
			content=e.getCars(format);
		}
		else if (strcmp(item,"time")==0)
		{
		    Environment &e = Application::getMainEnvironment();
		    content = e.getCurrentTime(format);
		}
		else if (strcmp(item,"track")==0)
		{
           content=Application::getMainEnvironment().getTrack(format);
		}
		else if (strcmp(item,"tracks")==0)
			content=Environment::getLoadableTracks(format);
		else if (strcmp(item,"sensors")==0)
			content=Application::getAllSensors().getDocument(format);
		else if (strcmp(item,"sensordata")==0)
		{ // getting data from LIDAR
			const char *value=request.getGET("name");
			if (value==NULL)
			{
                  sendTextResponse(
   "To get sensordata, you must specify the name of the LIDAR device."
			    ,r);
			   return;
			}
			else
			{
		   // get the sensor.
		   Sensor* s=Application::getAllSensors().getByName(value);
			   if (s==NULL)
			   {
                     sendTextResponse(
"Attempt to get LIDAR sensor data failed because no sensor exists by the specified name."
			       ,r);
				 return;
			   }
			   else
			   {
			      Lidar *lidar=dynamic_cast<Lidar*>(s);
				 if (lidar==NULL)
				 {
					sendTextResponse(
			        "A sensor was found with the specified name but it is not LIDAR."
			       ,r);
					return;
				 }
				 else
				 {
				  ostringstream oss;
				  const char * numpoints=request.getGET("numpoints");
				  int numpointsInt = lidar->getNumberOfPoints();

				    if (numpoints!=NULL)
				        numpointsInt=atoi(numpoints);

                    if (formatPtr==NULL)
                       format=string("txt");

				    Application::SubmitJobForMainThread(new LidarScanJob(oss,lidar,numpointsInt,format),true);

				    HTTPResponse response;
				    response.setHeader("Content-type","text/plain");
				    string content=oss.str();
				    response.setContent(content.c_str(),content.length());
				    response.writeTo(*(r->s));
				    return;
				 }
			   }
			}
		}
		else if (strcmp(item,"frame")==0)
		{ // frame from a video device
		    const char *value=request.getGET("name");
			if (value==NULL)
			{
                  sendTextResponse(
			    "To get a frame, you must specify the name of the video device."
			    ,r);
			   return;
			}
			else
			{
		 	   Sensor *s=Application::getAllSensors().getByName(value);
			   if (s==NULL)
			   {
                     sendTextResponse(
"Attempt to get a frame failed because no sensor exists by the specified name."
			       ,r);
				 return;
			   }
			   else
			   {
			      VideoDevice *video=dynamic_cast<VideoDevice*>(s);
				 if (video==NULL)
				 {
					sendTextResponse(
"Attempt to get a frame failed because a sensor exists by that name but it is not a video device."
			       ,r);
					return;
				 }
				 else
				 {
				   //  cout << "DocServer asking to update video frame"<<endl;
				    video->updateBufferedFrame();
				   // cout << "DocServer getting video frame"<<endl;
				    BitmapImage & img=video->getLatestFrame();
				    cout << "DocServer got frame."<<endl;
				    int len;
				    cout << "DocServer going to get image as bitmap file contents."<<endl;
                        char *bitmapData=
					    img.getAsBitmapFileContents(&len);
                   cout << "DocServer got bitmap file contents."<<endl;

                    if (bitmapData==NULL)
                    {
                        sendTextResponse("Problem getting image data."
			             ,r);
					     return;
                    }
				    HTTPResponse response;
				    response.setHeader("Content-type","image/bmp");
				    response.setContent(bitmapData,len);
				    response.writeTo(*(r->s));
				    cout << "DocServer sent bitmap response back to client."<<endl;
				    free(bitmapData);
				    cout << "DocServer freed bitmapData."<<endl;
				    return;
				 }
			   }
			}

		   sendContent=false;
		}
		else if (strcmp(item,"property")==0)
		{
			const char *value=request.getGET("name");
			if (value==NULL)
			{
                  sendTextResponse(
			    "To get a property, you must specify the name."
			    ,r);
			   return;
			}
			else
			{
		 	   content=Environment::getEnvironmentProperty(value);
			   sendTextResponse(content.c_str(),r);
			   return;
			}
			sendContent=false;
		}
		else if (strcmp(item,"wiremodel")==0)
		{
		  const char * filename = request.getGET("filename");
		  const char * type = request.getGET("type");

		  if (type!=NULL)
		  {
		    Environment &e = Application::getMainEnvironment();
		    ComplexObject *obj = e.createComplexObject(type);

              if (obj==NULL)
		    {
		       sendTextResponse(
			      "Unable to load model.  Check console for details."
			      ,r);
			  return;
		    }
		    else
		    {
			   string s = obj->getAsLines(string(format));
		        sendTextResponse(
			      s.c_str()
			      ,r);

		        delete obj;
		    } // end if else.
		     return;
		  } // end if type!=NULL

		  if (filename==NULL)
		  {
		    string content=
				 string("filename or type must be specified to get a wiremodel.  ")+
				 string("drone, roboticcar are valid type values.");

		     sendTextResponse(content.c_str()
			    ,r);

			return;
		  }
		  else
		  {
		     RRSMComplexObjectLoader loader;

                ComplexObject *obj = loader.loadFrom(filename);

              if (obj==NULL)
		    {
		       sendTextResponse(
			      "Unable to load model from file.  Check console for details."
			      ,r);
			  return;
		    }
		    else
		    {
			  string s = obj->getAsLines(string(format));

		        sendTextResponse(
			      s.c_str()
			      ,r);

		        delete obj;
			   return;
		    }
		  } // end if

		}
		else
		{
		  char message[200];
	     	sprintf(message,
"Invalid item value '%s' must be tracks or sensors.",item);
		     sendTextResponse(message,r);
			sendContent=false;
			return;
		}
		if (sendContent)
		{
			HTTPResponse response;
			if (format=="xml")
                   response.setHeader("Content-type","text/xml");
	          else if (format=="JSON")
				response.setHeader("Content-type","application/json");
			else
                  response.setHeader("Content-type","text/plain");

	         response.setContent(content.c_str(),content.size());
		    response.writeTo(*(r->s));
		}
	}
}

void handleSETPathRequest(HTTPRequest &request,DocRequest *r)
{
 const char *item = request.getGET("item");
 const char*value=request.getGET("value");

  if (item!=NULL)
  {
    if (strcmp(item,"stop")==0)
    {
      Application::setDriveTorqueQuantity(0);
	  Application::setSteeringTargetAngle(0);
	   sendTextResponse("1",r);
       return;
    }
  }

  if (value==NULL)
  {
	 sendTextResponse("value must be specified in any set request.",r);
	 return;
  }
  else
  {
	 float val;
	  sscanf(value,"%f",&val);
	  if (item==NULL)
	  {
		  sendTextResponse("item must be specified in any set request.",r);
	       return;
	  }
	  else if (strcmp(item,"drive")==0)
	  {
		  Application::setDriveTorqueQuantity(val);
		  sendTextResponse("1",r);
		  return;
	  }
	  else if (strcmp(item,"steering")==0)
	  {
		  Application::setSteeringTargetAngle(val);
		  sendTextResponse("1",r);
		  return;
	  }
	  else if (strcmp(item,"time")==0)
	  {
         int intVal = (int)val;

          if (intVal<0)
             sendTextResponse(
            "Can't go back in time.  The time value must be positive.",r);
		  else
		  {
		     Environment &e=Application::getMainEnvironment();
		     const char *action = request.getGET("action");
		     if (action==NULL)
		        e.ticksPerformed(intVal);
             else
             {
		       if (string(action)=="reset")
		          e.setClock(intVal);
		       else
		           e.ticksPerformed(intVal);
             }

		       sendTextResponse("1",r);
		  }
		  return;
	  }
	  else if (strcmp(item,"uimode")==0)
	  { // set the user interface mode.

		bool bringtofront=false;
          const char * frontStr = request.getGET("bringtofront");
		 if (frontStr!=NULL)
		 {
			if (strcmp(frontStr,"0")!=0)
		         bringtofront=true;
		 }
          Job *job = new SwitchUIModeJob((int)(val+0.5),bringtofront);
		Application::SubmitJobForMainThread(job);
            sendTextResponse("1",r);
		  return;
	  }
	  else if (strcmp(item,"iniproperty")==0)
	  {
		  const char *name=request.getGET("name");
		  if (name==NULL)
		  {
               sendTextResponse("Unable to set property.",r);
			return;
		  }
		  else
		  {
		     Environment::setINIProperty(name,val);
			sendTextResponse("1",r);
			return;
		  }
	  }
	  else
	  {
		  sendTextResponse(
			  "item must be set to 'drive' or 'steering'.",r);
		  return;
	  }
  }
}

void handleRemovePathRequest(HTTPRequest &request,DocRequest *r)
{
 const char *item = request.getGET("item");

 if (strcmp(item,"sensors")==0)
 {
   Application::getAllSensors().clearNonEssentialSensors();
    sendTextResponse(
    "1",r);
    return;
 }
 else if (strcmp(item,"track")==0)
 {
    //cout << "DocServer got request to clear track."<<endl;
    Application::getMainEnvironment().clearWithExceptions();
    //cout << "DocServer got the track cleared."<<endl;
    sendTextResponse(
    "1",r);
    return;
 }
 else
 {
     sendTextResponse(
    "The item parameter must be specified.",r);
	return;
 }
}

void handleLoadPathRequest(HTTPRequest &request,DocRequest *r)
{
 HTTPResponse response;
 const char *item = request.getGET("item");
 const char*value=request.getGET("value");

   if (item==NULL)
   {
     sendTextResponse(
     "item must be specified in any load request.",r);
	return;
   }
   else if (strcmp(item,"track")==0)
   {
     if (value==NULL)
	{
	  sendTextResponse(
"value must be specified when requesting to load a track."
		  ,r);
	  return;
	}
	else
	{
       // try loading the specified track.
       bool success=true;

       char filename[300];
       sprintf(filename,"tracks/%s",value);
       //Application::SubmitJobForMainThread(new TrackLoaderJob(string(filename)),true);
       Application::getMainEnvironment().loadTrackFrom(string(filename));

	  if (success)
		 sendTextResponse("1",r);
	  else
		 sendTextResponse("0",r);
	  return;
	}
   }
   else if (strcmp(item,"backdrop")==0)
   {
     if (value==NULL)
	{
	  sendTextResponse(
            "value must be set to an id when requesting to load a backdrop."
		  ,r);
	  return;
	}
	else
	{
       // try loading the specified track.
       bool success=safeLoadBackdropFrom(string(value));

	  if (success)
		 sendTextResponse("1",r);
	  else
		 sendTextResponse("0",r);
	  return;
	}
   }
   else
   {
     sendTextResponse(
	"Invalid item for removal.  Valid item values include track.",r);
	return;
   }
}

void getCommaSeparatedListOfString(const char *s,int *index,vector<string> *strings)
{
  string curString;
  int i=0;

  for (i=(*index);s[i]!='\0'&&s[i]!=';';i++)
  {
     if (s[i]==',')
	{
        strings->push_back(string(curString));
	   curString="";
	   continue;
	}
	curString+=s[i];
  }
  if (curString!="")
	  strings->push_back(string(curString));

  if (s[i]==';')
	  (*index)=i+1;
  else
      (*index)=i;
}

PhysicalObject * getPhysicalObjectFrom(const char *s,int *index)
{
  vector<string> strings;

  getCommaSeparatedListOfString(s,index,&strings);

  vector<string>::iterator it = strings.begin();

   if (it==strings.end())
   {
	   return NULL;
   }

  string type(*it);

  PhysicalObject * result=NULL;

  if (type=="pylon")
  {
     Pylon *p = new Pylon();
	p->setToRandomBrightness();
     result=p;
  }
  else if (type=="ramp")
  {
     result=new Ramp();
  }
  else if (type=="stop")
  {
     result=new StopSign();
  }
  else if (type=="light")
  {
    result=new TrafficLight();
  }
  else if (type=="verticalwall")
  {
     result = new VerticalWall();
  }
  else if (type=="maincar")
  {
	  RoboticCar * car = Application::getMainCar();
	  car->resetPositionAndSpeed();
	  result=car;
  }
  else
  {
	cout << "Unsupported object type: "<<type<<endl;
     return NULL;
  }

  for (it++;it!=strings.end();it++)
  {
	  string propAssignment(*it);
	  string::size_type pos = propAssignment.find("=");
	  if (pos==string::npos)
		  break;
	  else
	  {
	     string name = propAssignment.substr(0,pos);
		string value=propAssignment.substr(pos+1);
		double doubleValue;
		if (name=="coords")
		{
           VerticalWall * wall = dynamic_cast<VerticalWall*>(result);

           if (wall!=NULL)
           {
              wall->setProperty(name,value);
           }
		}
		else
		{
		    sscanf(value.c_str(),"%lf",&doubleValue);

		    result->setProperty(name,doubleValue);
		}
	  }
  }

  return result;
}

void handleADDPathRequest(HTTPRequest &request,DocRequest *r)
{
 HTTPResponse response;
 const char *item = request.getParam("item");
 Environment &e = Application::getMainEnvironment();

 if (item==NULL)
 {
     sendTextResponse(
     "item must be specified in any add request.",r);
	return;
 }
 else if (strcmp(item,"trackobject")==0)
 {
   const char *type=request.getGET("type");
    if (type==NULL)
    {
     sendTextResponse(
     "type must be specified in any add trackobject request.",r);
    }
    else if (strcmp(type,"pylon")==0 || strcmp(type,"stop")==0
	     || strcmp(type,"light")==0 || strcmp(type,"ramp")==0)
    {
	    PhysicalObject *po=NULL;

		if (strcmp(type,"pylon")==0)
		{
	        Pylon *p = new Pylon();
	           p->setToRandomBrightness();
                po=p;
		}
		else if (strcmp(type,"stop")==0)
		{
		   po = new StopSign();
		}
		else if (strcmp(type,"light")==0)
		{
			po = new TrafficLight();
		}
		else if (strcmp(type,"ramp")==0)
		{
			po = new Ramp();
		}

		// loop through parameters.
		const std::list<Parameter>& params = request.getGETParameters();

		for (std::list<Parameter>::const_iterator it=params.begin();
			it!=params.end();it++)
		{
		 const Parameter &p=(*it);
		 double value = 0;

		   if (sscanf(p.getValue(),"%lf",&value)==1)
		      po->setProperty(p.getName(),value);
		}
		Application::getMainEnvironment().add(po);
		sendTextResponse("1",r);
    }
    else
    {
         sendTextResponse(
        "Invalid trackobject type.  Valid type values include pylon,stop,light, and ramp.",r);
    }
    return;
 }
 else if (strcmp(item,"track")==0)
 { // add a collection of track objects.
    const char *value=request.getParam("value");

    if (value==NULL)
    {
       sendTextResponse(
        "value must be specified in any add track request.",r);
	  return;
    }
    else
    {
      int i=0;

	 //cout << "value = "<<value<<endl;

	  // loop through objects(split by semicolons).
       while (1)
	  {
	    // get object type.
	    PhysicalObject *po = getPhysicalObjectFrom(value,&i);
	    if (po!=NULL)
	    {
		    // if it isn't already in the environment, add it.
		    if (po!=Application::getMainCar())
		       e.add(po);
	    }
	    else
		    break; // get out of the while loop.
	  }

       sendTextResponse(
        "1",r);
	  return;
    }
 }
 else
 {
     sendTextResponse(
     "Invalid item.  Valid item values include trackobject, or track",r);
	return;
 }
}

void handleDronePathRequest(HTTPRequest &request,DocRequest *r)
{
 HTTPResponse response;
 const char *action = request.getGET("action");
 Environment &e = Application::getMainEnvironment();

  if (action==NULL)
  {
     sendTextResponse(
"action must be specified when requesting anything with drone."
	  ,r);
	return;
  }
  else
  {
	  RoboticCar * car = Application::getMainCar();

	  const char *indexStr = request.getGET("car_id");
	  if (indexStr!=NULL)
	  {
		car = e.getRoboticCarByID(string(indexStr));
		if (car==NULL)
		{
			sendTextResponse(
				"No car at the specified id.",r);
			return;
		}
	  }
	  if (strcmp(action,"landASAP")==0)
	  {
	     car->landDroneASAP();
		sendTextResponse("1",r);
		return;
	  }
	  else if (strcmp(action,"landOnCar")==0)
	  {
	  	car->landDroneOnCar();
		sendTextResponse("1",r);
		return;
	  }
	  else if (strcmp(action,"follow")==0)
	  {
	     car->releaseDrone();
	     RoboticHelicopter * drone=car->getReleasedDrone();
	     float elevation = 2000;
	     const char *value=request.getGET("elevation");
	     if (value!=NULL)
	     {
            sscanf(value,"%f",&elevation);
	     }
	     drone->follow(car,elevation);

	  }
	  else if (strcmp(action,"hover")==0)
	  {
         if (car!=NULL)
		 {
			 const char *value=request.getGET("elevation");
			 if (value==NULL)
			 {
			    sendTextResponse("elevation must be specified.",r);
			 }
			 else
			 {
			   float val;
				sscanf(value,"%f",&val);
				double elevation = val;


				  car->hoverDroneAt(val);
				sendTextResponse("1",r);
			 }
			 return;
		 }
		 else
		 {
             sendTextResponse("Main car not found.",r);
		   return;
		 }
	  }
	  else
	  {
	       sendTextResponse(
            "invalid action must be 'hover'."
	  ,r);
		  return;
	  }
  }
}

void handleRequest(DocRequest *r)
{
  HTTPRequest request(*(r->s));

  // handle the request.
  string path=request.getRequestFileNamePath();
  HTTPResponse response;

  //cout << "DocServer got request."<<endl;
  if (path=="/get")
  {
	handleGETPathRequest(request,r);
  }
  else if (path=="/add")
  {
     handleADDPathRequest(request,r);
  }
  else if (path=="/set")
  {
	handleSETPathRequest(request,r);
  }
  else if (path=="/load")
  {
    handleLoadPathRequest(request,r);
  }
  else if (path=="/remove")
  {
     handleRemovePathRequest(request,r);
  }
  else if (path=="/drone")
  {
    handleDronePathRequest(request,r);
  }
  else if (path=="/") // redirect to a useful document.
  {
       response.setHTTPResponseCode(301); // redirect.
	  response.setHeader("Location","interfaces.thtml");
	  response.setHeader("Content-type","text/html");

	string content;

	  content="<html><a href=\"interfaces.thtml\">Click here.</a></html>";
	  response.setContent(content.c_str(),content.size());
	  response.writeTo(*(r->s));
	  return;
  }
  else // general file document request
  {
	  // open the associated file.
       string filename="templates/public_html"+path;
	  vector<char> contents;

	  FILE *fp=fopen(filename.c_str(),"r");

	  if (fp!=NULL) // if file exists.
	  {
		  fclose(fp);
		  VariableCollection *c=new VariableCollection();
            const list<Parameter>& params=request.getGETParameters();
		  list<Parameter>::const_iterator it = params.begin();
		  for (;it!=params.end();it++)
		  {
		    c->add(new Constant(string("get.")+string(it->getName())
			    ,string(it->getValue())));
		  }
		  c->add(new Constant(string("headers.Host")
			  ,request.getHost()));

		  VariableCollectionStack vars;
		  vars.push(c);
		  THTMLProcessor::getProcessedFileContents(filename,
			 vars ,contents);

		  response.setHeader("Content-type",
			  HTTPResponse::getRecommendedMimeFor(filename));

		  char *buf=(char*)malloc(contents.size());
		  for (unsigned int i=0;i<contents.size();i++)
			  buf[i]=contents[i];

		  response.setContent(buf,contents.size());

		  response.writeTo(*(r->s));
		  free(buf);
	  }
	  else // not able to open file.
	  {
		  handleFileNotFound(filename,r);
	  }
  }

  delete r;
}

void handleIncomingRequests(ServerSocket *ss)
{
  if (ss==NULL)
  {
    cout << "handleIncomingRequests called with NULL ss."<<endl;
    return;
  }

  while (1)
  {
	Socket *s=ss->acceptClient();

	if (s==NULL)
		break;

	DocRequest *r=new DocRequest(s);

	Thread::startThread((void (*)(void*))handleRequest,r);
  }
}

DocServer::DocServer(): ss(port)
{
   Thread::startThread((void (*)(void*))handleIncomingRequests,&ss);
}

void DocServer::setDocServerPort(int port)
{
  if (port<0)
  {
    cout << "The docserver-port must be positive."<<endl;
    return;
  }

  DocServer::port=port;
}

int DocServer::getDocServerPort()
{
  return port;
}
