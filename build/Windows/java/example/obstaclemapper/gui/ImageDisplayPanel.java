package gui;

import graphics.*;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Dimension;

import javax.swing.JPanel;

public class ImageDisplayPanel extends JPanel
{
 private QuickAccessImage img;
 
  public ImageDisplayPanel()
  {
    this(null);
  }

  public ImageDisplayPanel(QuickAccessImage img)
  {
     this.img = img;
	 setPreferredSize(new Dimension(150,100));
  }
  
  public void setQuickAccessImage(QuickAccessImage img)
  {
     this.img = img;
	 repaint();
  }
 
  public void paint(Graphics g)
  {
    if (img==null)
	    return;

	double r1=getWidth()/(double)getHeight();
	double r2=img.getWidth()/(double)img.getHeight();
    int w,h;
    
	  if (r1>r2) // moved to narrow height panel
	  {
	     h=getHeight();
		 w=(int)(r2*h);
	  }
	  else
	  {
	     w=getWidth();
		 h=(int)(w/r2);
	  }
	
     // unstretched.
     g.drawImage(img.getInternalImage(),0,0,w,h,null);
	 g.setColor(Color.BLACK);
	 if (r1>r2)
	 {
		g.fillRect(w,0,getWidth()-w,getHeight());
	 }
	 else
	 {
		g.fillRect(0,h,getWidth(),getHeight()-h);	 
	 }
	 		 
     
  }
  
}