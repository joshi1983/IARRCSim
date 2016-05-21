package com.iarrc.gui;

import com.iarrc.roboticsio.IARRCGeneralInterfaceAdapter;
import javax.swing.*;
import java.awt.*;


public class ControlsPanel extends JPanel
{
 private IARRCGeneralInterfaceAdapter generalInterface;

  public ControlsPanel(IARRCGeneralInterfaceAdapter generalInterface)
  {
     this.generalInterface=generalInterface;
	 setLayout(new GridLayout(2,1));
	 add(new SteeringControlPanel(generalInterface));
	 add(new SpeedControlPanel(generalInterface));
  }
 
 
}