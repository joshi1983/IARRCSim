#include "TCPIPRobotController.hpp"
#include "../threads/Thread.hpp"

/*
    ServerSocket steeringSocket;
    ServerSocket driveSocket;
*/

int TCPIPRobotController::steeringport=3200;
int TCPIPRobotController::driveport=3201;

struct serverSocketThreadInfo
{
  RoboticCar *car;
  ServerSocket *ss;

  serverSocketThreadInfo(RoboticCar *car,ServerSocket *ss): car(car),ss(ss)
{
  
}
  ~serverSocketThreadInfo()
  {
    delete ss;
  }
};

struct socketThreadInfo
{
  RoboticCar *car;
  Socket *s;

  socketThreadInfo(RoboticCar *car,Socket *s): car(car),s(s)
  {
  
  }

  ~socketThreadInfo()
  {

     delete s;
  }
};

void processSteering(socketThreadInfo *info)
{
 int bufLen=200;
 char buf[201];
 int numBytesRead = info->s->read(buf,bufLen);

  if (numBytesRead>1)
  {
	  char c = buf[0];
	  if (c=='S')
	  { // ie. "S 5.34\0"
	  
	  }
	  else
	  { // ie. "GET /?num=5.34 HTTP..."
	  
	  }
	// call method on car.
  }

  delete info;
}

void processDrive(socketThreadInfo *info)
{
  // read in the command
    // call method on car.

	delete info;
}

void processSteeringHandler(serverSocketThreadInfo *info)
{
  while (1)
  {
    Socket *s=info->ss->acceptClient();
    socketThreadInfo *sti=new socketThreadInfo(info->car,s);

    Thread::startThread((void (*)(void *))processSteering,sti);
  }
  delete info;
}

void processDriveHandler(serverSocketThreadInfo *info)
{
  while (1)
  {
    Socket *s=info->ss->acceptClient();
    socketThreadInfo *sti=new socketThreadInfo(info->car,s);

       Thread::startThread((void (*)(void *))processDrive,sti);
  }
  delete info;
}

TCPIPRobotController::TCPIPRobotController(RoboticCar *car): RobotController(car),
  steeringSocket(steeringport),driveSocket(driveport)
{
  startServerThreads();
}

void TCPIPRobotController::update(Environment * e)
{

}

bool TCPIPRobotController::setGlobalSteeringPort(int port)
{
	if (port<0)
		return false;

  steeringport=port;
  return true;
}

bool TCPIPRobotController::setGlobalDrivePort(int port)
{
  if (port<0)
	return false;

  driveport=port;
  return true;
}

int TCPIPRobotController::getGlobalDrivePort()
{
  return driveport;
}

int TCPIPRobotController::getGlobalSteeringPort()
{
  return steeringport;
}

void TCPIPRobotController::startServerThreads()
{
  serverSocketThreadInfo *sti=new serverSocketThreadInfo(car,&driveSocket);
  Thread::startThread((void (*)(void *))processDriveHandler,sti);

  sti=new serverSocketThreadInfo(car,&steeringSocket);
  Thread::startThread((void (*)(void *))processSteeringHandler,sti);
}


