package com.iarrc.gui;

import com.iarrc.roboticsio.*;
import javax.swing.*;
import java.awt.*;

public class UltrasonicDisplayPanel extends JPanel
{
 private UltrasonicSensor sensor;
 
  public UltrasonicDisplayPanel(UltrasonicSensor sensor)
 {
   this.sensor=sensor;
 }
 
 public void paint(Graphics g)
 {
   g.setColor(Color.WHITE);
   g.fillRect(0,0,getWidth(),getHeight());
   g.setColor(Color.GREEN);
   g.fillRect(0,0,(int)(getWidth()*sensor.getDepth()/sensor.getMaxDepth()),getHeight());
   g.setColor(Color.BLACK);
   g.drawString("Depth: "+sensor.getDepth(),getWidth()/2,25);
   
   g.drawString(sensor.getName(),0,25);
 }
}