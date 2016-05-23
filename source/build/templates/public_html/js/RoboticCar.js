
var cars=new Array();function RoboticCar(id,maincar)
{this.id=id;this.maincar=maincar;}
RoboticCar.prototype.launchDrone=function()
{var url="";}
function getIDForCarDroneDemo()
{for(var i=0;i<cars.length;i++)
{if(cars[i].maincar==false)
return cars[i].id;}
return false;}
function updateDroneControlExamples()
{var newID=getIDForCarDroneDemo();var e=document.getElementById("droneControlExamples");if(newID==false)
{var msg=document.createTextNode("The following commands won't work because there is no car1 in the environment right now.");e.insertBefore(msg,e.firstChild);return;}
var links=e.getElementsByTagName("a");for(var i=0;i<links.length;i++)
{var href=links[i].href;href=href.replace("car1",newID);links[i].href=href;}}
function updateCars()
{var url="/get?item=cars&format=xml";var doc=getXMLDocumentFromURL(url);cars=new Array();var vehicles=doc.getElementsByTagName("vehicle");for(var i=0;i<vehicles.length;i++)
{var v=vehicles[i];var id=v.getAttribute("id");var maincar=parseInt(v.getAttribute("maincar"));if(maincar==0)
maincar=false;else
maincar=true;cars.push(new RoboticCar(id,maincar));}}
function initControls()
{updateCars();updateDroneControlExamples();}