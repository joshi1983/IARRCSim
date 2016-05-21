package com.iarrc.gui;

import com.iarrc.roboticsio.IARRCGeneralInterfaceAdapter;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 A SpeedControlPanel is a component to help users control steering
*/
public class SpeedControlPanel extends JPanel implements MouseListener,MouseMotionListener
{
 private static final double MAX_VALUE=10;
 private IARRCGeneralInterfaceAdapter generalInterface;
 private double value;
 
   public SpeedControlPanel(IARRCGeneralInterfaceAdapter generalInterface)
   {
     this.generalInterface = generalInterface;
	 addMouseListener(this);
	 addMouseMotionListener(this);
   }
   
  public void paint(Graphics g)
  {
    SteeringControlPanel.paintGauge(g,this,(value+MAX_VALUE)/MAX_VALUE*0.5);
    g.setColor(Color.BLACK);
	g.drawString("Speed Control",0,25);
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
  
  public void updateFor(int x)
  {
	 double xRatio = x/(double)getWidth();
	 
	 value = (xRatio-0.5)*MAX_VALUE*2;
	 repaint();
	 try
	 {
	   generalInterface.setSpeedValue(value);
	 }
	 catch (Exception exc)
	 {
	   exc.printStackTrace();
	 }
  }
  
  public void mousePressed(MouseEvent e)
  {

	      updateFor(e.getX());
  }

  public void mouseReleased(MouseEvent e) 
  {
  }
  
   public void 	mouseDragged(MouseEvent e)
   {
        updateFor(e.getX());
   }

   public void 	mouseMoved(MouseEvent e) 
   {
   }
}