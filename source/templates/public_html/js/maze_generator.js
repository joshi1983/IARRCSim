
var displacement={"x":0,"y":0};var mazeScale=300;var useWalls=true;var numPointsPerLine=6;function setMazeScale(scale)
{mazeScale=scale;numPointsPerLine=Math.floor(mazeScale/40);}
function updateCellWidth()
{var id="cell_width";var select=document.getElementById(id);if(!select)
{alert("Internal error: can't get element with id: "+id+" so can't update cell width.");return;}
var val=select.options[select.selectedIndex].value;var num=parseFloat(val);if(isNaN(num)||num<1)
alert("Invalid maze cell size: "+val);else
setMazeScale(num);}
function makeUsingWalls()
{useWalls=true;}
function makeUsingPylons()
{useWalls=false;}
function updateMaterials()
{var select=document.getElementById("material");var val=select.options[select.selectedIndex].value;if(val=="w")
makeUsingWalls();else
makeUsingPylons();}
function transformX(x)
{return x*mazeScale+displacement.x;}
function transformY(y)
{return y*mazeScale+displacement.y;}
function replaceSimulatorTrackWithMaze()
{updateMaterials();updateCellWidth();var trackStrings=new Array("");if(!useWalls)
{var pylonPoints;pylonPoints=getPylonPoints();setStatusMessage("Composing Maze Track string");if(maze.grid.length>0)
{displacement.x=-mazeScale*maze.grid.length/2;displacement.y=-mazeScale*maze.grid[0].length/2;}
else
{displacement={"x":0,"y":0};}
for(var i=0;i<pylonPoints.length;i++)
{var pylonS="";if(trackStrings[trackStrings.length-1]!="")
pylonS=";";pylonS+="pylon,position-x="+transformX(pylonPoints[i].x)+",position-z="+transformY(pylonPoints[i].y);trackStrings[trackStrings.length-1]+=pylonS;if(trackStrings[trackStrings.length-1].length>5000)
{trackStrings.push("");}}}
else
{var walls=getWalls();for(var i=0;i<walls.length;i++)
{var wallS="";if(trackStrings[trackStrings.length-1]!="")
wallS=";";wallS+=walls[i].toSimFormat();trackStrings[trackStrings.length-1]+=wallS;if(trackStrings[trackStrings.length-1].length>5000)
{trackStrings.push("");}}}
{var maincar="";if(trackStrings[trackStrings.length-1]!="")
maincar=";";maincar+="maincar,position-x="+transformX(0.5)+",position-z="+transformY(0.5);var rotation=0;for(var i=0;i<4;i++)
{if(!maze.grid[0][0].sides[i])
{rotation=90-i*90;break;}}
maincar+=",rotation="+rotation;trackStrings[trackStrings.length-1]+=maincar;}
var endPoint=maze.getMaximumCountPoint();{px=0.5+endPoint.x;py=0.5+endPoint.y;var stopSignDescription=";stop,position-x="
+transformX(px)+",position-z="+transformY(py);var rotation=0;for(var i=0;i<4;i++)
{if(!maze.grid[endPoint.x][endPoint.y].sides[i])
{rotation=90-i*90;break;}}
stopSignDescription+=",rotation="+rotation;trackStrings[trackStrings.length-1]+=stopSignDescription;}
setStatusMessage("Clearing current track");var requestObject=getXMLHTTPRequestObject();requestObject.open("post","/remove?item=track",false);requestObject.send("");setStatusMessage("Sending new maze track to simulator");for(var i=0;i<trackStrings.length;i++)
{setStatusMessage("Sending new maze track to simulator(chunk "+(i+1)
+" of "+(trackStrings.length)+")");trackStrings[i]=encodeURI(trackStrings[i]);var requestObject=getXMLHTTPRequestObject();requestObject.open("post","/add?item=track",false);requestObject.send("value="+trackStrings[i]);}
setStatusMessage("Maze track was successfully sent to simulator");setTimeout("setStatusMessage('')",5000);}
function getWalls()
{var result=new Array();if(maze.grid.length==0)
return result;var startY=-1;for(var x=0;x<maze.grid.length;x++)
{startY=-1;for(var y=0;y<maze.grid[x].length;y++)
{if(maze.grid[x][y].sides[0])
{if(startY==-1)
startY=y;}
else
{if(startY!=-1)
{result.push(new Wall(transformX(1+x),transformY(startY),transformX(1+x),transformY(y)));startY=-1;}}}
if(startY!=-1)
{result.push(new Wall(transformX(1+x),transformY(startY),transformX(1+x),transformY(y)));startY=-1;}}
var startX=-1;for(var y=0;y<maze.grid[0].length;y++)
{for(var x=0;x<maze.grid.length;x++)
{if(maze.grid[x][y].sides[1])
{if(startX==-1)
startX=x;}
else
{if(startX!=-1)
{result.push(new Wall(transformX(startX),transformY(1+y),transformX(x),transformY(1+y)));startX=-1;}}}
if(startX!=-1)
{result.push(new Wall(transformX(startX),transformY(1+y),transformX(x),transformY(1+y)));startX=-1;}}
if(maze.grid[0].length>0)
{result.push(new Wall(transformX(0),transformY(0),transformX(0),transformY(maze.grid[0].length)));result.push(new Wall(transformX(0),transformY(0),transformX(maze.grid.length),transformY(0)));result.push(new Wall(transformX(0),transformY(maze.grid[0].length),transformX(maze.grid.length),transformY(maze.grid[0].length)));result.push(new Wall(transformX(maze.grid.length),transformY(0),transformX(maze.grid.length),transformY(maze.grid[0].length)));}
return result;}
function getPylonPoints()
{var result=new Array();if(maze.grid.length==0)
return result;for(var x=0;x<maze.grid.length;x++)
{for(var y=0;y<maze.grid[x].length;y++)
{if(maze.grid[x][y].sides[0])
addVerticalLinePoints(x,y,result);if(maze.grid[x][y].sides[1])
addHorizontalLinePoints(x,y,result);}}
if(maze.grid[0].length>0)
{for(var x=0;x<maze.grid.length;x++)
{addHorizontalLinePoints(x,-1,result);addHorizontalLinePoints(x,maze.grid[x].length-1,result);}
for(var y=0;y<maze.grid[0].length;y++)
{addVerticalLinePoints(-1,y,result);addVerticalLinePoints(maze.grid.length-1,y,result);}}
return result;}
function addVerticalLinePoints(x,y,results)
{for(var i=0;i<=numPointsPerLine;i++)
{var p=new Point(x+1,y+i/numPointsPerLine);if(findIndexOf(results,p)<0)
results.push(p);}}
function findIndexOf(points,point)
{for(var i=0;i<points.length;i++)
{if(points[i].x==point.x&&points[i].y==point.y)
return i;}
return-1;}
function addHorizontalLinePoints(x,y,results)
{for(var i=0;i<=numPointsPerLine;i++)
{results.push(new Point(x+i/numPointsPerLine,y+1));}}