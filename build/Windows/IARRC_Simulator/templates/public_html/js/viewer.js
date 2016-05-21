
var zoom=250;var pos={"x":0,"y":0,"z":-50};var rotation_angle=0;var rotation_speed=0.01;var lines=new Array();var display_centre={"x":100,"y":100};var mouse_lastpos=false;var mouse_down=false;function rotatePoint2D(x,y,angle)
{if(!angle)
angle=rotation_angle;var result={"x":x*Math.cos(angle)+y*Math.sin(angle),"y":-x*Math.sin(angle)+y*Math.cos(angle)};return result;}
function getCurrentRadius()
{return Math.sqrt(pos.x*pos.x+pos.z*pos.z);}
function updatePosAndDirection(desiredRadius)
{var radius=desiredRadius;if(!radius)
radius=getCurrentRadius();pos.x=radius*Math.sin(rotation_angle);pos.z=radius*Math.cos(rotation_angle);}
function animateRotation()
{rotation_angle+=rotation_speed;updatePosAndDirection();updateViewerDisplay();setTimeout("animateRotation()",30);}
function initSphere(cx,cy,cz,radius)
{for(var lat=-2;lat<2;lat++)
{var latR=lat*Math.PI/4;var y=cy+radius*Math.sin(latR);var smallRadius=radius*Math.cos(latR);for(var long=0;long<5;long++)
{var longR=long*Math.PI*2/5.0;var prevLongR=(long-1)*Math.PI*2/5.0;var start=new Point3D(cx+smallRadius*Math.sin(prevLongR),y,cz+smallRadius*Math.cos(prevLongR));var end=new Point3D(cx+smallRadius*Math.sin(longR),y,cz+smallRadius*Math.cos(longR));lines.push(new Line(start,end));}}}
function getViewerModelType()
{var e=document.getElementById("viewer_model_type");if(!e)
return false;else
return e.value;}
function getModelFilename()
{var e=document.getElementById("modelFile");if(!e)
{alert("Unable to get modelFile element.");return"";}
return e.value;}
function loadModelDataFromURL(url)
{var text=sendRequestForTextTo(url);var modelData=[];try
{modelData=eval(text);}
catch(e)
{alert("Unable to load model "+e);}
lines=new Array();loadLinesFromModelData(modelData);return false;}
function loadModelIntoViewer()
{var filename=getModelFilename();var url="/get?item=wiremodel&filename="+encodeURI(filename)+"&format=JSON";loadModelDataFromURL(url);}
function updateModelType(Type)
{var type="roboticcar";if(Type)
{type=Type;}
else
{var e=document.getElementById("type");type=e.options[e.selectedIndex].value;}
var url="/get?item=wiremodel&type="+encodeURI(type)+"&format=JSON";loadModelDataFromURL(url);}
function Point3D(x,y,z)
{this.x=x;this.y=y;this.z=z;}
Point3D.prototype.toX=function()
{var dx=this.x-pos.x;var dz=this.z-pos.z;var p=rotatePoint2D(dx,dz,Math.PI-rotation_angle);dz=p.y;dx=p.x;if(dz<0)
return false;return display_centre.x+zoom*dx/dz;}
Point3D.prototype.toY=function()
{var dy=this.y-pos.y;var dz=this.z-pos.z;var dx=this.x-pos.x;var p=rotatePoint2D(dx,dz,Math.PI-rotation_angle);dx=p.x;dz=p.y;if(dz<0)
return false;return display_centre.y-zoom*dy/dz;}
function Line(start,end)
{this.start=start;this.end=end;}
Line.prototype.draw=function(canvasContext)
{var sx=this.start.toX(),sy=this.start.toY(),ex=this.end.toX(),ey=this.end.toY();if(sx&&sy&&ex&&ey)
{canvasContext.moveTo(sx,sy);canvasContext.lineTo(ex,ey);}
else
{}}
function getCanvasElement()
{var e=document.getElementById("viewer3d");if(!e)
{alert("Unable to get element by id: viewer.  Unable to draw 3D display.");}
return e;}
function loadLinesFromModelData(modelData)
{try
{for(var i=0;i<modelData.length;i++)
{var line=modelData[i];if(line)
{lines.push(new Line(new Point3D(line[0].x,line[0].y,line[0].z),new Point3D(line[1].x,line[1].y,line[1].z)));}}}
catch(e)
{alert("loadLinesFromModelData problem: "+e);}}
function viewer3D_mousemove(event)
{if(!mouse_down)
return;if(!event)
event=window.event;var newPos={"x":event.pageX,"y":event.pageY};if(mouse_lastpos)
{var dy=newPos.y-mouse_lastpos.y;var dx=newPos.x-mouse_lastpos.x;pos.y+=dy*0.3;var r=getCurrentRadius()+dx;if(r>0)
{updatePosAndDirection(r);}}
mouse_lastpos=newPos;}
function viewer3D_mouseup(event)
{mouse_lastpos=false;mouse_down=false;}
function viewer3D_mousedown(event)
{if(!event)
event=window.event;mouse_lastpos={"x":event.pageX,"y":event.pageY};mouse_down=true;}
function init3DViewer()
{var canvas=getCanvasElement();var type=getViewerModelType();if(type)
updateModelType(type);else
loadLinesFromModelData(carlines);updateViewerDisplay();animateRotation();}
function updateViewerDisplay()
{var e=getCanvasElement();if(!e)
return;var canvasContext=e.getContext('2d');if(!canvasContext)
{return;}
display_centre.x=e.width/2;display_centre.y=e.height/2;canvasContext.fillStyle="#FFFFFF";canvasContext.fillRect(0,0,10000,10000);canvasContext.strokeStyle="#666666";for(var i=0;i<lines.length;i++)
{canvasContext.beginPath();lines[i].draw(canvasContext);canvasContext.stroke();canvasContext.closePath();}}