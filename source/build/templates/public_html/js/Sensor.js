
function Sensor(sensorE)
{this.updateWith(sensorE);}
Sensor.prototype.updateDisplay=function()
{var sensor_id="sensor_"+this.name;var e=document.getElementById(sensor_id);if(!e)
{e=document.createElement("div");e.setAttribute("id",sensor_id);setCSSClass(e,"sensorData");getSensorDataDisplays().appendChild(e);}
else
{removeChildrenFrom(e);}
var heading=document.createElement("h4");heading.appendChild(document.createTextNode(this.name));e.appendChild(heading);if(this.type=="video")
{var a=document.createElement("a");a.setAttribute("name",sensor_id);e.appendChild(a);var div=document.createElement("div");a=document.createElement("a");a.setAttribute("href","videodisplay.thtml?name="+this.name);a.appendChild(document.createTextNode("Orientation Controls"));div.appendChild(a);div.setAttribute("align","center");e.appendChild(div);}
else if(this.type=="ultrasonic")
{var divParent=document.createElement("div");divParent.setAttribute("width","100%");var div=document.createElement("div");setCSSClass(div,"ultrasonicDepth");var widthS=Math.round(100*(this.properties['depth']/this.properties['range-max']))+"%";div.style.backgroundColor="#88f";div.style.width=widthS;divParent.appendChild(div);e.appendChild(divParent);}
else if(this.type=="lidar")
{var a=document.createElement("a");a.setAttribute("href","lidardisplay.thtml?name="+this.name);a.appendChild(document.createTextNode("Detailed LIDAR Display"));e.appendChild(a);}}
Sensor.prototype.updateWith=function(sensorE)
{this.name=sensorE.getAttribute("name");this.type=sensorE.getAttribute("type");var properties=sensorE.getElementsByTagName("property");this.properties=new Array();for(var i=0;i<properties.length;i++)
{this.properties[properties[i].getAttribute("name")]=properties[i].getAttribute("value");}}