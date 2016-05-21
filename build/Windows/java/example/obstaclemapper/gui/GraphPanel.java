package gui;

import javax.swing.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;

public class GraphPanel extends JPanel
{
 private static final int MODE_POINTS=0;
 private static final int MODE_HISTOGRAM=1;
 private static final int NUM_MODES=2;
 
 private int maxNumToPlot;
 private LinkedList<Double> values=new LinkedList<Double>();
 private static int left=15;
 private static int top=15;
 private static int rightSpacing=15;
 private static int bottomSpacing=15;
 private String title="";
 private int mode=MODE_POINTS;
 private int numHistogramIntervals=15;

  
 public GraphPanel(int maxNumToPlot)
 {
    this.maxNumToPlot=maxNumToPlot;
	
	histogramMouseMotionListener mouseHandler=new histogramMouseMotionListener();
	addMouseListener(mouseHandler);
	addMouseMotionListener(mouseHandler);
 }
 
 class histogramMouseMotionListener extends MouseMotionAdapter implements MouseListener
 {
   private int oldX,oldY;
   private double numIntervals;
 
    public histogramMouseMotionListener()
	{
	   numIntervals = getNumHistogramIntervals();
	}
 
 	public void mouseClicked(MouseEvent me)
	{
		  oldX=me.getX();
	      oldY=me.getY();
	      nextMode();
	}
	
	public void mousePressed(MouseEvent me)
	{
	  oldX=me.getX();
	  oldY=me.getY();
	}
	
	public void mouseReleased(MouseEvent me)
	{
	  oldX=me.getX();
	  oldY=me.getY();
	}
	
	public void mouseExited(MouseEvent me)
	{
	}
	
	public void mouseEntered(MouseEvent me)
	{
	}
	   
    public void mouseDragged(MouseEvent me)
    {
         if (mode!=MODE_HISTOGRAM)
		   return;

		int dx = me.getX()-oldX;
		 
		 numIntervals = numIntervals*Math.pow(1.05,dx);
		 		 
		 if (numIntervals<3)
            numIntervals=3;
         if (numIntervals>maxNumToPlot)
            numIntervals=maxNumToPlot;
			
		 setNumHistogramIntervals((int)numIntervals);

		oldX=me.getX();
	    oldY=me.getY();       
    }
 }
 
 public void nextMode()
 {
     mode=(mode+1)%NUM_MODES;
	 repaint();
 }
 
 public void setNumHistogramIntervals(int num)
 {

	  
   numHistogramIntervals=num;
   repaint();
 }
 
 public int getNumHistogramIntervals()
 {
   return numHistogramIntervals;
 }
 
 public void showHistogram()
 {
    if (mode==MODE_HISTOGRAM)
	   return; // nothing to change.
	
    mode=MODE_HISTOGRAM;
	repaint();
 }
 
 public void showPoints()
 {
    if (mode==MODE_POINTS)
	   return; // nothing to change.
	   
    mode=MODE_POINTS;
	repaint();
 }
 
 public void setTitle(String title)
 {
    this.title=title;
 }
 
 public synchronized void paint(Graphics g)
 {
     g.setColor(Color.BLACK);
     g.fillRect(0,0,getWidth(),getHeight());
	 
	 // draw the axis lines.
	 g.setColor(Color.WHITE);
	 
	 g.drawString(this.title,getWidth()/2-3*title.length(),10);
	 
	 // draw the vertical axis.
	 g.drawLine(left,top,left,getHeight()-bottomSpacing);
	 
	 // draw the horizontal axis.
	 g.drawLine(left,getHeight()-bottomSpacing,getWidth()-rightSpacing,getHeight()-bottomSpacing);
	 
	 double maxVal=getMaxVal();
	 double minVal=getMinVal();
	 double dVal = maxVal-minVal;
	 
	 if (dVal==0) // done painting for now.
	     return; 
		 
		 		 
	 g.setColor(new Color(100,170,255));
		 
	 double scaleY = getGraphableHeight()/dVal;
	 if (mode==MODE_POINTS)
	 {
	   double scaleX = getGraphableWidth()/(double)maxNumToPlot;
	   int x=0;

		 // loop through plotting data points.
		 for (double val: values)
		 {
			int y=(int)(getHeight()-bottomSpacing-(val-minVal)*scaleY);
			// System.out.println("y="+y);
			 g.fillRect((int)(x*scaleX+left),y,1,1);
			 x++;
		 }
	 }
	 else
	 {
	    int[] counts = new int[numHistogramIntervals];
		
		for (int i=0;i<counts.length;i++)
		    counts[i]=0; // initialize all counts to 0.
		
	    // numHistogramIntervals
		for (double val: values)
		{
		   int index = (int)((val-minVal)*numHistogramIntervals/(maxVal-minVal));
		   
		   /* in the very rare case where floating point calculation error 
		   puts the number too high, keep index within array boundaries.
		   */
		   if (index>=numHistogramIntervals)
		       index = numHistogramIntervals-1;
		   else if (index<0)
		       index=0;
			   
		   counts[index]++;   
		}
		int max=-1;
		// get the maximum count.
		for (int i=0;i<counts.length;i++)
		{
		   if (counts[i]>max)
		      max=counts[i];
		}
		int w= 1+getGraphableWidth()/counts.length;
		
		// loop through graphing each bar.
		for (int i=0;i<counts.length;i++)
		{
			int y=(int)(top+getGraphableHeight()*(1-counts[i]/(double)max));
			int x=(int)(left+i*getGraphableWidth()/(double)counts.length);
			// System.out.println("y="+y);
			 g.fillRect(x,y,w,getGraphableHeight()+top-y);
		}
	 }
	 g.drawString(""+maxVal,0,20);
	 g.drawString(""+minVal,0,getHeight()-10);
	 //System.out.println("plotted points: "+values.size());
 }
 
 public int getGraphableWidth()
 {
   return getWidth()-left-rightSpacing;
 }
 
 public int getGraphableHeight()
 {
     return getHeight()-bottomSpacing-top;
 }
 
 public synchronized double getMaxVal()
 {
   double result=Double.MIN_VALUE;
   
    for (double v: values)
	   if (result<v)
          result = v;	  
		  
	return result;
 }
 
 public synchronized double getMinVal()
 {
   double result=Double.MAX_VALUE;
   
    for (double v: values)
	   if (result>v)
          result = v;	  
		  
	return result;
 }
 
 public synchronized void addValue(double val)
 {
    values.add(val);
	assert !values.isEmpty(); // if the value was added, it shouldn't be empty anymore.
	assert values.size()>0;

	if (values.size()>maxNumToPlot)
	   values.removeFirst();
	   
	repaint();
 }
 
 
}