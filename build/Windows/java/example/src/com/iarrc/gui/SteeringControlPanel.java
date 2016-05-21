package com.iarrc.gui;

import com.iarrc.roboticsio.IARRCGeneralInterfaceAdapter;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 A SteeringControlPanel is a component to help users control steering
*/
public class SteeringControlPanel extends JPanel implements MouseListener,MouseMotionListener
{
 private static final double MAX_VALUE=15;
 private IARRCGeneralInterfaceAdapter generalInterface;
 private double value;
  
   public SteeringControlPanel(IARRCGeneralInterfaceAdapter generalInterface)
   {
     this.generalInterface = generalInterface;
	 addMouseListener(this);
	 addMouseMotionListener(this);
   }
  
  public static void paintGauge(Graphics g,JPanel p,double ratio)
  {
    g.setColor(Color.WHITE);
	g.fillRect(0,0,p.getWidth(),p.getHeight());
	g.setColor(new Color(100,200,255));
	g.fillRect(0,0,(int)(p.getWidth()*ratio),p.getHeight());
	g.setColor(Color.BLACK);
	int middle_x = p.getWidth()/2;
	g.fillRect(middle_x-2,0,4,p.getHeight());
  }
  
  public void paint(Graphics g)
  {
    paintGauge(g,this,(value+MAX_VALUE)/MAX_VALUE*0.5);
	g.setColor(Color.BLACK);
	g.drawString("Steering Control",0,25);
  }
  
  public void updateFor(int x)
  {
     double xRatio = x/(double)getWidth();
	 
	 value = (xRatio-0.5)*MAX_VALUE*2;
	 repaint();
	 try
	 {
	   generalInterface.setSteeringValue(-value);
	 }
	 catch (Exception exc)
	 {
	   exc.printStackTrace();
	 }
  }
  
  public void mouseClicked(MouseEvent e)
  {
  }
  
  public void mouseEntered(MouseEvent e)
  {
  }
  
  public void mouseExited(MouseEvent e)
  {
  }
  
  public void mousePressed(MouseEvent e)
  {
    updateFor(e.getX());
  }

  public void mouseReleased(MouseEvent e) 
  {
       updateFor(e.getX());
  }
  
     public void mouseDragged(MouseEvent e)
   {
     updateFor(e.getX());
   }

   public void 	mouseMoved(MouseEvent e) 
   {
   }
}