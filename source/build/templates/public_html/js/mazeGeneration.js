
var width=25,height=25;var showsolution=false;var maze=false;function setSolutionVisibility()
{var box=document.getElementById("show_solution");if(showsolution==box.checked)
return;showsolution=!showsolution;drawMaze();}
function initDisplay()
{generateMaze();}
function invalidDimension(d)
{if(!d)
return"must be specified.";if(d=="")
return"must be specified.";var val=parseFloat(d);if(isNaN(val))
return"must be a number.";if(val<=0)
return"must be bigger than 0.";return false;}
function updateDimensions()
{var widthStr=document.getElementById("width").value;var heightStr=document.getElementById("height").value;var msg=invalidDimension(widthStr);if(msg)
return"width - "+msg;msg=invalidDimension(heightStr);if(msg)
return"height - "+msg;width=parseFloat(widthStr);height=parseFloat(heightStr);return false;}
function generateMaze()
{var msg=updateDimensions();if(msg)
{alert(msg);return;}
maze=new Maze(width,height);maze.makeRandomMaze(0,0);drawMaze();}
function drawMaze()
{maze.fillTable(document.getElementById("mazeDisplay"));}
function MazeCell()
{this.sides=new Array();this.allWalls();this.visited=false;}
MazeCell.prototype.allWalls=function()
{for(var i=0;i<4;i++)
{this.sides[i]=true;}}
var TDborders=new Array("0px","1px solid black");function getBorderValueFromBoolean(val)
{if(val)
return TDborders[1];else
return TDborders[0];}
MazeCell.prototype.getAsTD=function()
{var result=document.createElement("td");result.style.borderRight=getBorderValueFromBoolean(this.sides[0]);result.style.borderBottom=getBorderValueFromBoolean(this.sides[1]);result.style.borderLeft=getBorderValueFromBoolean(this.sides[2]);result.style.borderTop=getBorderValueFromBoolean(this.sides[3]);if(showsolution)
{if(this.issolutioncell)
result.style.backgroundColor="#dfd";}
return result;}
function Maze(width,height)
{this.width=width;this.height=height;this.grid=new Array();for(var x=0;x<this.width;x++)
{this.grid[x]=new Array();for(var y=0;y<this.height;y++)
{this.grid[x][y]=new MazeCell();}}}
Maze.prototype.drawRandomSolutionFrom=function(p)
{var cell=this.grid[p.x][p.y];this.setAllCellsNotVisited();while(cell.count>0)
{cell.issolutioncell=true;cell.visited=true;var accessibleNeighbourPoints=this.getUnvisitedNeighboursAccessible(p.x,p.y);if(accessibleNeighbourPoints.length==0)
{alert("no unvisited neighbours left");break;}
var found=false;for(var i=0;i<accessibleNeighbourPoints.length;i++)
{var p2=accessibleNeighbourPoints[i];var cell2=this.grid[p2.x][p2.y];if(cell2.count==cell.count-1)
{cell=cell2;p=p2;found=true;break;}}
if(!found)
{alert("unable to find next cell to go to.");break;}}}
Maze.prototype.setAllCellsNotVisited=function()
{for(var x=0;x<this.width;x++)
for(var y=0;y<this.height;y++)
this.grid[x][y].visited=false;}
Maze.prototype.getMaximumCountPoint=function()
{var maxC=-1;var maxPoint=new Point(0,0);var fringe=new Array(new Point(0,0));this.setAllCellsNotVisited();this.grid[0][0].count=0;this.grid[0][0].visited=true;while(fringe.length>0)
{var p=fringe.shift();var pv=this.grid[p.x][p.y];var unvisitedNeighbours=this.getUnvisitedNeighboursAccessible(p.x,p.y);for(var i=0;i<unvisitedNeighbours.length;i++)
{var cell=this.grid[unvisitedNeighbours[i].x][unvisitedNeighbours[i].y];cell.visited=true;cell.count=pv.count+1;fringe.push(unvisitedNeighbours[i]);maxPoint=unvisitedNeighbours[i];}}
this.drawRandomSolutionFrom(maxPoint);return maxPoint;}
Maze.prototype.fillTable=function(tableE)
{removeChildrenFrom(tableE);var tbody=document.createElement("tbody");var maxPoint=this.getMaximumCountPoint();for(var y=0;y<this.height;y++)
{var tr=document.createElement("tr");for(var x=0;x<this.width;x++)
{var td=this.grid[x][y].getAsTD();if(x==maxPoint.x&&y==maxPoint.y)
td.style.backgroundColor="#ddf";tr.appendChild(td);}
tbody.appendChild(tr);}
tableE.appendChild(tbody);}
var offset_points=new Array(new Point(1,0),new Point(0,1),new Point(-1,0),new Point(0,-1));function getSideIndexFromOffset(x_offset,y_offset)
{if(x_offset==1)
return 0;else if(x_offset==-1)
return 2;else if(y_offset==1)
return 1;else
return 3;}
function Point(x,y)
{this.x=x;this.y=y;}
Maze.prototype.removeWall=function(x1,y1,x_offset,y_offset)
{this.grid[x1][y1].sides[getSideIndexFromOffset(x_offset,y_offset)]=false;var x2=x1+x_offset,y2=y1+y_offset;this.grid[x2][y2].sides[getSideIndexFromOffset(-x_offset,-y_offset)]=false;}
Maze.prototype.safeIsUnvisited=function(x,y)
{if(x<0||y<0)
return false
if(x>=this.width||y>=this.height)
return false;return!this.grid[x][y].visited;}
Maze.prototype.getUnvisitedNeighboursAccessible=function(x,y)
{var cell=this.grid[x][y];var result=new Array();for(var i=0;i<4;i++)
{if(!cell.sides[i])
{if(this.safeIsUnvisited(x+offset_points[i].x,y+offset_points[i].y))
{result.push(new Point(x+offset_points[i].x,y+offset_points[i].y));}}}
return result;}
Maze.prototype.getUnvisitedNeighbours=function(x,y)
{var result=new Array();var count=0;for(var i=0;i<offset_points.length;i++)
{var x2=offset_points[i].x,y2=offset_points[i].y;if(this.safeIsUnvisited(x+x2,y+y2))
{result[count++]=new Point(x+x2,y+y2);}}
return result;}
Maze.prototype.makeRandomMaze=function()
{var x=0,y=0;var stack=new Array();while(true)
{this.grid[x][y].visited=true;var unvisitedneighbours=this.getUnvisitedNeighbours(x,y);if(unvisitedneighbours.length>0)
{var index=Math.floor(Math.random()*unvisitedneighbours.length);var point=unvisitedneighbours[index];this.removeWall(x,y,point.x-x,point.y-y);x=point.x;y=point.y;stack.push(point);}
else if(stack.length==0)
break;else
{var p=stack.pop();x=p.x;y=p.y;}}}