package com.iarrc.gui;

import com.iarrc.roboticsio.*;
import javax.swing.*;
import java.awt.*;

public class LidarDisplayPanel extends JPanel
{
 private LidarSensor sensor;
 
 public LidarDisplayPanel(LidarSensor sensor)
 {
   this.sensor=sensor;
 }
 
 public void paint(Graphics g)
 {
   g.setColor(Color.WHITE);
   g.fillRect(0,0,getWidth(),getHeight());
   g.setColor(Color.BLACK);
   int radius = Math.min(getWidth(),getHeight())/2;
   double radiusScale = radius/sensor.getMaxDepth();
   int cx=getWidth()/2,cy=getHeight()/2;
     g.drawOval(cx-radius,cy-radius,radius*2,radius*2);
     // draw circle.
   
   try
   {
   // draw detected points.
   double[] depths=sensor.getDepths();

   double rangeScan = sensor.getRangeScan(); 
    // number of degrees for full scan. could be 270
	
   double angle=-rangeScan*Math.PI/180*0.5;
   double angleChange = (rangeScan/depths.length)*Math.PI/180;
	
	// draw the V shape to show what area is not scanned by the Lidar sensor.
   g.drawLine(cx,cy,cx+(int)(radius*Math.cos(angle)),cy+(int)(radius*Math.sin(angle)));
   double mangle = angle+depths.length*angleChange;
   g.drawLine(cx,cy,cx+(int)(radius*Math.cos(mangle)),cy+(int)(radius*Math.sin(mangle)));
   
   
	// plot dots.
    for (int i=0;i<depths.length;i++)
    {
      int x,y; // pixel coordinates.
      double distance = depths[i]*radiusScale;
	  
	  x=cx+(int)(distance*Math.cos(angle));
	  y=cy+(int)(distance*Math.sin(angle));
	  
	  g.fillOval(x,y,2,2);
	  angle+=angleChange;
    }
   }
   catch (Exception e)
   {
     e.printStackTrace();
     g.drawString("Problem: "+e,cx,cy);
   }
   g.setColor(Color.BLACK);
   g.drawString(sensor.getName(),0,25);
 }
 
}