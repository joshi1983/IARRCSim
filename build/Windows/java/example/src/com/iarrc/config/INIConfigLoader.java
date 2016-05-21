package com.iarrc.config;

import java.io.*;
import java.util.HashMap;

public class INIConfigLoader extends ConfigLoader
{
  public boolean accepts(File f)
  {
      String filename = f.getName();
	  filename = filename.toLowerCase();
	  if (filename.endsWith(".ini"))
	     return true;
	  else
	     return false;
  }
  
  public static void loadINIDataInto(InputStream in,HashMap<String,String> map) 
   throws IOException
  {
  BufferedReader br = new BufferedReader(new InputStreamReader(in));
  String line = null;
  
       // loop through lines of the file.
     while ((line=br.readLine())!=null)
     {
         line = line.trim(); // trim any whitespaces at the beginning or end of the line.
		 if (line.equals(""))
		     continue; // don't do anything with blank lines.
		
		 int indexOfPound = line.indexOf('#');
		 
		 if (indexOfPound>=0) // if using a comment, remove the comment from line.
		    line = line.substring(0,indexOfPound).trim();
			
		 if (line.equals(""))
		     continue; // don't do anything with blank lines.
			 
		 int indexOfEquals = line.indexOf('=');
		 if (indexOfEquals<0)
		 {
		    System.err.println("configuration INI syntax problem in line: "+line);
			continue;
		 }
		 String propertyName = line.substring(0,indexOfEquals).trim();	
		 String value = line.substring(indexOfEquals+1).trim();
		 
		 map.put(propertyName,value);
		 
     } // end while
	 
  }
  
  public Config load(InputStream in) throws IOException
  {
	Config result = new Config();
	HashMap<String,String> map = new HashMap<String,String>();
	
	 loadINIDataInto(in,map);
	
     // loop through lines of the file.
     for (String propertyName: map.keySet())
     {
		 result.set(propertyName,map.get(propertyName)); 
     } // end while
	 
	 result.loadConfigurationPropertiesFromIARRCServer();
	 
	 return result;
  }
}