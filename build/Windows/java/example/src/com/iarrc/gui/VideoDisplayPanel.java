package com.iarrc.gui;

import com.iarrc.roboticsio.*;
import javax.swing.*;
import java.awt.*;

public class VideoDisplayPanel extends JPanel
{
 private VideoSensor sensor;
 
  public VideoDisplayPanel(VideoSensor sensor)
 {
   this.sensor=sensor;
 }
 
 public void paint(Graphics g)
 {
    sensor.paintDisplay(g,0,0,getHeight());
	
	g.setColor(Color.BLACK);
	g.drawString(sensor.getName(),0,25);
 }
}