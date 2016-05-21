package com.iarrc.config;

import java.io.*;
import java.util.LinkedList;

/**
A class for loading configuration information from a general source.
*/
public abstract class ConfigLoader
{
 private static LinkedList<ConfigLoader> loaders = getAllConfigLoaders();

  private static LinkedList<ConfigLoader> getAllConfigLoaders()
  {
   LinkedList<ConfigLoader> result = new LinkedList<ConfigLoader>();
   
    result.add(new INIConfigLoader());
	
    return result;
  }
 
  public static Config loadFrom(File f) throws IOException
  {
    for (ConfigLoader L: loaders)
	   if (L.accepts(f))
	      return L.load(new FileInputStream(f));
	
	throw new IOException("Unsupported file extension for configuration with: "+f);
  }

  public abstract boolean accepts(File f);
  
  public abstract Config load(InputStream in) throws IOException;

}