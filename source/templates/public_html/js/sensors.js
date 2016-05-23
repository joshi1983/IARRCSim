
var sensors=new Array();var delayTime=700;var auto_refresh=true;var timeout=false;var allsensors_httprequest=false;function setAutoRefresh(on)
{if(on==auto_refresh)
return;auto_refresh=on;if(on)
{updateSensorDisplay();if(!timeout)
timedUpdate();}
else
{if(timeout)
clearTimeout(timeout);timeout=false;}}
function updateAutoRefresh()
{var box=document.getElementById("autorefreshbox");setAutoRefresh(box.checked);}
function getSensorByName(name)
{for(var i=0;i<sensors.length;i++)
if(sensors[i].name==name)
return sensors[i];return false;}
function updateVideoSensorsDisplay()
{for(var i=0;i<sensors.length;i++)
if(sensors[i].type=="video")
sensors[i].updateDisplay();}
function replaceSensorData(sensorsDoc)
{if(!sensorsDoc)
{alert("replaceSensorData called with null sensorsDoc.");return;}
sensors=new Array();var sensorsE=sensorsDoc.getElementsByTagName("device");for(var i=0;i<sensorsE.length;i++)
{var sensorE=sensorsE[i];var sensor=new Sensor(sensorE);sensors[i]=sensor;sensor.updateDisplay();}
SensorsUpdated();}
function AllSensorsUpdate(sensorsDoc)
{if(!sensorsDoc)
{alert("AllSensorsUpdate called with null sensorsDoc.");return;}
var sensorsE=sensorsDoc.getElementsByTagName("device");for(var i=0;i<sensorsE.length;i++)
{var sensorE=sensorsE[i];var sensor=getSensorByName(sensorE.getAttribute("name"));if(!sensor)
{replaceSensorData(sensorsDoc);return;}
else
{sensor.updateWith(sensorE);if(sensor.type=="ultrasonic")
sensor.updateDisplay();else if(sensor.type!="video"&&sensor.type!="lidar")
alert("type: "+sensor.type);}}
SensorsUpdated();}
function AllSensorsResponseUpdated()
{if(allsensors_httprequest.readyState==4)
{if(allsensors_httprequest.responseXML)
{AllSensorsUpdate(allsensors_httprequest.responseXML);updateTheAllSensorsData();}
else
{}}}
function updateTheAllSensorsData()
{var url="get?item=sensors&=uniqueId="+(new Date()).getTime();allsensors_httprequest=getXMLHTTPRequestObject();if(!allsensors_httprequest)
{alert("Your browser either doesn't support XMLHttpRequest or has it disabled.  "
+"Please enable it.");return;}
allsensors_httprequest.onreadystatechange=AllSensorsResponseUpdated;allsensors_httprequest.open("GET",url,true);allsensors_httprequest.send();}
function getSensorDataDisplays()
{return document.getElementById("sensorDisplays");}
function getSensorListDisplay()
{return document.getElementById("sensorDisplay");}
function SensorsUpdated()
{var e=getSensorListDisplay();removeChildrenFrom(e);for(var i=0;i<sensors.length;i++)
{var li=document.createElement("li");var a=document.createElement("a");a.setAttribute("href","#sensor_"+sensors[i].name);a.appendChild(document.createTextNode(sensors[i].name+" ("+sensors[i].type+")"));li.appendChild(a);e.appendChild(li);}
updateVideoSensorsDisplay();}
function updateSensorDisplay()
{updateTheAllSensorsData();timedUpdate();}
function timedUpdate()
{if(!auto_refresh)
return;updateVideoSensorsDisplay();timeout=setTimeout("timedUpdate()",delayTime);}
function setSteeringValue(val)
{sendRequestNonBlocking("set?item=steering&value="+encodeURI(val)+"&uniqueId="+(new Date()).getTime());}
function setDriveValue(val)
{var url="set?item=drive&value="+encodeURI(val)+"&uniqueId="+(new Date()).getTime();sendRequestNonBlocking(url);}
function driveMouseDown(evt)
{if(!evt)
evt=window.event;var e=getAncestorWithTagName(getFiringObject(evt),"td");var width=getElementWidth(e);var x=getXRelativeToElement(e,evt);var value=50*(x-width/2)/width;setDriveValue(value);plotPosition(e,x/width);}
function steeringMouseDown(evt)
{if(!evt)
evt=window.event;var e=getAncestorWithTagName(getFiringObject(evt),"td");var width=getElementWidth(e);var x=getXRelativeToElement(e,evt);var value=20*(x-width/2)/width;setSteeringValue(-value);plotPosition(e,x/width);}
function plotPosition(e,ratio)
{var width=getElementWidth(e);var x=width*ratio;removeChildrenFrom(e);var div=document.createElement("div");setCSSClass(div,"positionIndicator");div.style.position="absolute";div.style.left=(getClientX(e)+x)+"px";div.style.top=getClientY(e)+"px";div.style.height=getElementHeight(e)+"px";e.appendChild(div);}