The java folder contains all source code for IARRC_Sim_API.jar and an application 
called obstaclemapper which uses IARRC_Sim_API.jar to communicate with the simulator.

folder:
src

- source code for IARRC_Sim_API.jar
- IARRC_Sim_API.jar archives the classes needed to 
 retrieve sensor data and send back motor control commands.


obstaclemapper

- source code for an application that uses IARRC_Sim_API.jar to connect with the simulator for reading sensor data and sending back motor control commands.






Compiling and Running

To run the Java code, you need Java installed 
on your computer and the PATH environment variable set to 
include the path to your java installation's bin folder.

Since you most likely want to compile the source files also, you will 
need Java SDK(Software Development Kit) installed instead of Java JRE(Runtime Edition).  
The SDK includes all capabilities of JRE but more.  It comes with a compiler called javac.

The config/config.ini file will also need its sim-install property set to 
the directory where you've extracted the simulator.

If you are in Windows, the .bat files should run the programs.  
If you are in Ubuntu, use the .sh files to run the programs.


Problems?

If your terminal says java or javac is not a recognized command, 
you must still set the PATH environment variable to include Java's bin folder.  
If Java is not installed, install it.

If your terminal shows a stack trace starting with "java.io.FileNotFoundException:", 
you must still update the sim-install property in config/config.ini to the directory where the simulator was extracted.
