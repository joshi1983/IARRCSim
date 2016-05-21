package gui;

import graphics.*;
import java.awt.Graphics;
import javax.swing.JPanel;

public class VideoDisplay extends JPanel
{
 private QuickAccessImage img;
 
  public VideoDisplay()
  {
    this(null);
  }

  public VideoDisplay(QuickAccessImage img)
  {
     this.img = img;
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
		
     // unstretched.
     g.drawImage(img.getInternalImage(),0,0,null);
  }
  
}