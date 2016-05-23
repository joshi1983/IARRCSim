
var rotationvector=false;var dragging=false;function regularSimulatorUpdates()
{rotationvector.updateSimulatorProperties();setTimeout("regularSimulatorUpdates()",500);}
function valOrZero(n)
{n=parseFloat(n);if(!isNaN(n))
return n;else
return 0;}
function intializeXYZ()
{rotationvector.xDegrees=valOrZero(getSimulatorProperty("device-"+getSensorName()+"-rotation-x"));rotationvector.yDegrees=valOrZero(getSimulatorProperty("device-"+getSensorName()+"-rotation-y"));rotationvector.zDegrees=valOrZero(getSimulatorProperty("device-"+getSensorName()+"-rotation-z"));rotationvector.updateInputs(false);}
function getInputX()
{return document.getElementById("vertical_input");}
function getCanvasX()
{return document.getElementById("vertical_display");}
function getInputY()
{return document.getElementById("horizontal_input");}
function getCanvasY()
{return document.getElementById("horizontal_display");}
function getInputZ()
{return document.getElementById("twist_input");}
function getCanvasZ()
{return document.getElementById("twist_display");}
function horizontalInputFieldUpdate()
{rotationvector.updateYFromInputElement();}
function horizontalMouseDown(evt)
{rotationvector.updateY(evt);dragging=true;}
function horizontalMouseMove(evt)
{if(dragging)
rotationvector.updateY(evt);}
function horizontalMouseUp(evt)
{rotationvector.updateY(evt);dragging=false;}
function verticalMouseDown(evt)
{rotationvector.updateX(evt);dragging=true;}
function verticalMouseMove(evt)
{if(dragging)
rotationvector.updateX(evt);}
function verticalMouseUp(evt)
{rotationvector.updateX(evt);dragging=false;}
function verticalInputFieldUpdate()
{rotationvector.updateXFromInputElement();}
function twistMouseDown(evt)
{rotationvector.updateZ(evt);dragging=true;}
function twistMouseMove(evt)
{if(dragging)
rotationvector.updateZ(evt);}
function twistMouseUp(evt)
{rotationvector.updateZ(evt);dragging=false;}
function twistInputFieldUpdate()
{rotationvector.updateZFromInputElement();}
function initRotationVector()
{rotationvector=new RotationVector(0,0,0,getInputX(),getInputY(),getInputZ(),getCanvasX(),getCanvasY(),getCanvasZ(),"device-"+getSensorName()+"-rotation-x","device-"+getSensorName()+"-rotation-y","device-"+getSensorName()+"-rotation-z");}
function initOrientationControlDisplay()
{initRotationVector();rotationvector.updateInputs();intializeXYZ();regularSimulatorUpdates();}