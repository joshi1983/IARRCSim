package gui;

import geometry.*;
import mapping.ObstacleMap;
import mapping.LidarSensorPerspective;

import javax.swing.*;
import java.awt.*;

/**
Displays an ObstacleMap
*/
public class MapDisplayPanel extends JPanel
{
 private ObstacleMap map;
 private MapOnlyDisplay display;
 private JLabel status;
 
  public MapDisplayPanel(ObstacleMap map)
  {
    this.map = map;
	display = new MapOnlyDisplay();
	status = new JLabel();
	
	setLayout(new BorderLayout());
	add(display,BorderLayout.CENTER);
	add(status,BorderLayout.SOUTH);
	
  }
  
  public void setEstimatedRotationAngle(double rotationAngleRadians)
  {
      status.setText("Rotation: "+(rotationAngleRadians*180/Math.PI)+" degrees");
  }
  
  private void paintDistanceGrid(Graphics g,Vector3D offset,double scale)
  {
    double gsize=15;
	int size = 15;
    double[][] distances = map.createObstacleDistanceGrid(size,gsize);
    LidarSensorPerspective perspective = map.getLidarSensorPerspective();
	int radius = (int)(scale*gsize/2)+1;
	 // plus 1 to ensure it stays visible with at least 1 pixel regardless of scale.
	
     for (int x=0;x<distances.length;x++)
	 {
	    for (int y=0;y<distances[0].length;y++)
		{
		   double distance = distances[x][y];
		   // get colour from the distance value.
           Color c = new Color(100,0,0);
		   if (distance<127)
		      c=new Color(100,0,2*(int)(127-distance));
			  
			Vector2D v = new Vector2D((x-size/2)*gsize,(size*3/10-y)*gsize);
			v.rotateBy(-Math.PI*3/4+perspective.getRotationRadians());
			v=v.plus(perspective.getPositionAsVector2D());
			double x2d=scale*(v.getX()-offset.getX()),
                 z2d=scale*(v.getY()-offset.getZ());
				 
			g.setColor(c);
			g.fillOval((int)x2d-radius,(int)z2d-radius,radius*2,radius*2);
		}
	 }
  
  }
  
  class MapOnlyDisplay extends JPanel
  {
  public void paint(Graphics g)
  {
    // clear the background.
    g.setColor(Color.BLACK);
	g.fillRect(0,0,getWidth(),getHeight());
  
    if (map==null) // don't draw anything if there is no map.
	   return;
	   
	Vector3D minrange=map.getMinRange();
	Vector3D maxrange=map.getMaxRange();
	
	double dx=(maxrange.getX()-minrange.getX()),dz=(maxrange.getZ()-minrange.getZ());
    double scale=1;
	double xscale =1; 
	double zscale = 1;
	if (dx!=0)
	   xscale=getWidth()/dx;
	if (dz!=0)
	   zscale = getHeight()/dz;
	   
	scale = Math.min(xscale,zscale);

    Vector3D centre = new Vector3D(getWidth()/2/scale,0,getHeight()/2/scale);	
	Vector3D offset = (minrange.plus(maxrange).times(0.5)).minus(centre);
	
	paintDistanceGrid(g,offset,scale);
	
	map.paint(g,offset,scale);
  }
  }
}