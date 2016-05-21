#ifndef TCPIP_ROBOT_CONTROLLER_HEADER
#define TCPIP_ROBOT_CONTROLLER_HEADER

#include "RobotController.hpp"
#include "../io/ServerSocket.hpp"

/**
For controlling a robot using TCP/IP
*/
class TCPIPRobotController: public RobotController
{
 private:
    ServerSocket steeringSocket;
    ServerSocket driveSocket;

    void startServerThreads();
    static int steeringport;
    static int driveport;

 public:
   TCPIPRobotController(RoboticCar *car);
	 virtual void update(Environment * e);
    int getSteeringPort() const;
    int getDrivePort() const;

  static bool setGlobalSteeringPort(int port);
  static bool setGlobalDrivePort(int port);

  static int getGlobalSteeringPort();
  static int getGlobalDrivePort();
};


#endif
