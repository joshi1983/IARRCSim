package com.iarrc.roboticsio;

import java.util.*;
import org.w3c.dom.*;
import java.net.URL;
import javax.xml.parsers.*;
import java.io.InputStream;

public class XMLHelper
{

  public static Document getDocFromURL(String url) throws Exception
  {
    return getDoc(new URL(url));
  }

  public static Document getDoc(URL url) throws Exception
  {
    return getDoc(url.openStream());
  }
  
  public static Document getDoc(InputStream in) throws Exception
  {
    DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
    DocumentBuilder db = dbf.newDocumentBuilder();
    Document doc = db.parse(in);
	return doc;
  }
}