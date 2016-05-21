package com.iarrc.config;

import java.util.*;
import java.io.*;

/**
 Represents the configurable information used by the Java-based IARRC client
*/
public class Config
{
 private static final String PROPERTY_INSTALL_DIRECTORY="sim-install";
 private static final String PROPERTY_CONFIG_DIRECTORY="sim-config";
 private static final String PROPERTY_DOCSERVER_HOST = "docserver-host";
 private static final String PROPERTY_DOCSERVER_PORT = "docserver-port";
 
 private HashMap<String,String> properties;
 
 public Config()
 {
    init();
 }
 
 private void init()
 {
   properties = new HashMap<String,String>(); 
   
   // set defaults?
 }
 
 /**
 
 */
 public void loadConfigurationPropertiesFromIARRCServer() throws IOException
 {
   String configFilename = getSimulatorConfigDirectory()+"\\config.ini";
   HashMap<String,String> map = new HashMap<String,String>();
  
    INIConfigLoader.loadINIDataInto(new FileInputStream(configFilename),map);  
  String port = map.get(PROPERTY_DOCSERVER_PORT);
  if (port!=null)
  {
     set(PROPERTY_DOCSERVER_PORT,port);
  }
    
 }
 
 /**
   Gets the port used for an initial connection with the simulation server
 */
 public int getDocServerPort()
 {
   String portStr = get(PROPERTY_DOCSERVER_PORT);
   String host = get(PROPERTY_DOCSERVER_HOST);
   
   if (portStr==null)
   {
     if (host!=null) // extract the port number from the host.
     {
	   int index = host.indexOf(":");
	    if (index>0)
	    {
	      // extract from host string.  ie. "localhost:2600"
	      portStr = host.substring(index+1);
	    }
     }
   }
   
   if (portStr!=null)
      return Integer.parseInt(portStr);
   else
   {	 
      return 2600;
   }
 }
 
 public String getDocServerHost()
 {
   String result = "localhost";
   String host = get(PROPERTY_DOCSERVER_HOST);
   
   if (host!=null)
      return host;
   
   return result;
 }
 
 public String getDocServerAddress()
 {
    return "http://"+getDocServerHost()+":"+getDocServerPort()+"/";
 }
 
 /**
 Sets the specified property to the specified value
 */
 public void set(String propertyName,String value)
 {
    properties.put(propertyName,value);
 }
 
 /**
 Returns the value of the specified property
 */
 public String get(String propertyName)
 {
   return properties.get(propertyName);
 }
 
 public void setSimulatorInstallDirectory(String installDir)
 {
   set(PROPERTY_INSTALL_DIRECTORY,installDir);
 }
 
 public void setSimulatorConfigDirectory(String configDir)
 {
   set(PROPERTY_CONFIG_DIRECTORY,configDir);
 }
 
 public String getSimulatorInstallDirectory()
 {
   return properties.get(PROPERTY_INSTALL_DIRECTORY);
 }
 
 public String getSimulatorConfigDirectory()
 {
  String result = properties.get(PROPERTY_CONFIG_DIRECTORY);
  
   // if configuration directory known, return it.
   if (result!=null) 
      return result;
	  
   // configuration directory not known so 
   // let's try deriving it from install directory.	  
   String installDir = getSimulatorInstallDirectory();
   
   if (installDir==null) 
   {
      return null;
	  // can't derive since install directory is unknown
   }
   else
   {
      return installDir+"\\config";
   }
 }
 
 public String getAsINIContents()
 {
   StringBuffer result = new StringBuffer();
   
    for (String propertyName: properties.keySet())
    {
	    result.append(propertyName+": "+properties.get(propertyName)+"\r\n");
	}
   
    return result.toString();
 }
 
 public String toString()
 {
   return getAsINIContents();
 }
 
}