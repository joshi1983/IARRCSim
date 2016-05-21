package gui;

import graphics.*;
import javax.swing.JPanel;
import java.util.LinkedList;
import java.awt.Image;
import java.awt.GridLayout;
import java.awt.Dimension;

public class ImageSetPanel extends JPanel
{
 private LinkedList<QuickAccessImage> images=new LinkedList<QuickAccessImage>();
  
  public ImageSetPanel()
  {
     setPreferredSize(new Dimension(150,100));
  }
  
  public synchronized void add(QuickAccessImage img)
  {
     images.add(img);
	 updateDisplay();
  }
  
  public synchronized void updateDisplay()
  {
     removeAll();
     setLayout(new GridLayout(images.size(),1));
	 for (QuickAccessImage img: images)
	 {
	     add(new ImageDisplayPanel(img));
	 }
	 updateUI();
     System.out.println("ImageSetPanel height = "+getHeight());
  }
  
  public synchronized void clear()
  {
     images.clear();
	 updateDisplay();
  }
}