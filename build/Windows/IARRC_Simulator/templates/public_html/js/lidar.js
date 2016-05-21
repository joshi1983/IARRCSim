
var depths=new Array();var scale=0.6;var clientX=0;var clientY=0;var mouseDown=false;var autorefreshdata=false;var autorefreshPointSize=2;var fullResPointSize=1;var regularPointSize=1;var drawingdisplay=false;var lowResRayCount=200;var lidardataupdate_httprequest=false;function setAutorefreshData(doit)
{if(doit==autorefreshdata)
return;autorefreshdata=doit;if(autorefreshdata)
{setStatusMessage("Auto-refreshing data at low resolution");regularPointSize=autorefreshPointSize;updateLidarDataQuick();}
else
{regularPointSize=fullResPointSize;setStatusMessage("Refreshing display in full resolution");updateLidarData();}}
function updateLidarDataQuick()
{var url="http://localhost:"+getDocumentServerPort()
+"/get?item=sensordata&name="+getSensorName()+"&numpoints="+lowResRayCount;updateLidarData(url);}
function updateLidarDataFromString2(lidarData)
{depths=new Array();var lines=lidarData.split("\n");for(var i=0;i<lines.length;i++)
{var num=parseFloat(lines[i]);if(isNaN(num))
break;else
depths[i]=num;}}
function lidarDataResponseUpdated()
{if(lidardataupdate_httprequest.readyState==4)
{var s=lidardataupdate_httprequest.responseText;if(drawingdisplay)
return;updateLidarDataFromString2(s);updateDisplay(true);if(autorefreshdata)
{updateLidarDataQuick();}}}
function updateLidarData(Url)
{if(drawingdisplay)
return;var url=false;if(Url)
url=Url;else
{url="http://localhost:"+getDocumentServerPort()
+"/get?item=sensordata&name="+getSensorName();}
url=url+"&uniqueId="+(new Date()).getTime();lidardataupdate_httprequest=getXMLHTTPRequestObject();if(!lidardataupdate_httprequest)
{setStatusMessage("XMLHttpRequest must be supported and enabled.  "
+"Check that your browser is both.");return;}
lidardataupdate_httprequest.onreadystatechange=lidarDataResponseUpdated;lidardataupdate_httprequest.open("GET",url,true);lidardataupdate_httprequest.send();}
function setScale(scaleRatio)
{scale=scaleRatio;if(scale<0.1)
scale=0.1;else if(scale>5)
scale=5;updateZoomMessage(""+Math.floor(scale*100)+"%");updateDisplay(true);}
function updateZoomMessage(msg)
{var zoomDisplay=document.getElementById("zoomDisplay");removeChildrenFrom(zoomDisplay);zoomDisplay.appendChild(document.createTextNode(msg));}
function getCanvasElement()
{var canvas=document.getElementById("display");return canvas;}
function reportHTML5Incompatibility()
{setStatusMessage("Your browser does not support HTML5 which is "
+"required for LIDAR visualization.  "
+"We recommend Google Chrome or Firefox.");}
function updateDisplay(hideStatus)
{if(drawingdisplay)
return;if(depths.length<1)
return;drawingdisplay=true;try
{if(!hideStatus)
setStatusMessage("Updating canvas display");var canvas=getCanvasElement();var angleRange=getAngleRangeInDegrees();var max_depth=getMaximumDepth();if(!canvas.getContext)
{reportHTML5Incompatibility();}
else
{var ctx=canvas.getContext('2d');var offset_x=250,offset_y=250;var angle_offset=angleRange*Math.PI/180*0.5+Math.PI/2;ctx.beginPath();ctx.fillStyle="white";ctx.fillRect(0,0,1000,1000);ctx.fillStyle="black";{ctx.strokeStyle="black";var r=max_depth*scale;var angle=-angle_offset;var x=Math.floor(r*Math.cos(angle));var x=offset_x+x,y=offset_y+Math.floor(r*Math.sin(angle));ctx.moveTo(offset_x,offset_y);ctx.lineTo(x,y);angle=angleRange*Math.PI/180-angle_offset;x=offset_x+Math.floor(r*Math.cos(angle));y=offset_y+Math.floor(r*Math.sin(angle));ctx.moveTo(offset_x,offset_y);ctx.lineTo(x,y);}
for(var i=0;i<depths.length;i++)
{var angle=i*angleRange*Math.PI/180/depths.length-angle_offset;var r=depths[i]*scale;var x=offset_x+Math.floor(r*Math.cos(angle)),y=offset_y+Math.floor(r*Math.sin(angle));if(!isNaN(x)&&!isNaN(y))
ctx.fillRect(x,y,regularPointSize,regularPointSize);if(!hideStatus)
if(i%30==0)
{setStatusMessage("Updating canvas display: "+
Math.floor(100*i/depths.length)+"% complete.");}}
ctx.closePath();ctx.stroke();}
if(!hideStatus)
setStatusMessage("Full resolution static display");}
catch(e)
{alert("Problem - "+e);}
drawingdisplay=false;}
function canvasMouseDown(evt)
{if(!evt)
evt=window.event;lastX=evt.clientX;lastY=evt.clientY;mouseDown=true;}
function canvasMouseUp(evt)
{if(!evt)
evt=window.event;lastX=evt.clientX;lastY=evt.clientY;mouseDown=false;}
function canvasMouseMove(evt)
{if(!evt)
evt=window.event;var newX=evt.clientX;var newY=evt.clientY;if(mouseDown)
{var dx=newX-lastX;var dy=newY-lastY;setScale(Math.pow(1.05,dx)*scale);}
lastX=newX;lastY=newY;}
function initDisplay()
{updateLidarData();setAutorefreshData(document.getElementById("autorefreshdata").checked);var canvas=getCanvasElement();setScale(Math.min(0.5*getElementWidth(canvas),getElementHeight(canvas))/getMaximumDepth());}