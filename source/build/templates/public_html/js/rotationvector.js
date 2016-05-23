
function drawRotationAngleCircle(ctx,left,top,diameter,angleDegrees)
{if(!ctx)
return;var radius=diameter/2;var angleRadians=angleDegrees*Math.PI/180;var cx=left+radius;var cy=top+radius;ctx.beginPath();ctx.fillStyle="white";ctx.fillRect(left,top,diameter,diameter);ctx.strokeStyle="black";ctx.arc(cx,cy,radius,0,Math.PI*2,true);var x,y;x=cx+radius*Math.sin(angleRadians);y=cy-radius*Math.cos(angleRadians);ctx.moveTo(cx,cy);ctx.lineTo(x,y);ctx.closePath();ctx.stroke();}
function RotationVector(xDegrees,yDegrees,zDegrees,inputElementX,inputElementY,inputElementZ,canvasX,canvasY,canvasZ,xproperty,yproperty,zproperty)
{this.xDegrees=xDegrees;this.yDegrees=yDegrees;this.zDegrees=zDegrees;this.inputElementX=inputElementX;this.inputElementY=inputElementY;this.inputElementZ=inputElementZ;this.canvasX=canvasX;this.canvasY=canvasY;this.canvasZ=canvasZ;this.xproperty=xproperty;this.yproperty=yproperty;this.zproperty=zproperty;this.xneeds_sim_update=false;this.yneeds_sim_update=false;this.zneeds_sim_update=false;}
RotationVector.prototype.updateSimulatorProperties=function()
{if(this.xneeds_sim_update)
setINIProperty(this.xproperty,this.xDegrees);if(this.yneeds_sim_update)
setINIProperty(this.yproperty,this.yDegrees);if(this.zneeds_sim_update)
setINIProperty(this.zproperty,this.zDegrees);this.xneeds_sim_update=false;this.yneeds_sim_update=false;this.zneeds_sim_update=false;}
RotationVector.prototype.updateX=function(evt)
{if(!evt)
evt=window.event;var canvasE=this.canvasX;var x=getXRelativeToElement(canvasE,evt);var y=getYRelativeToElement(canvasE,evt);x-=canvasE.width/2;y-=canvasE.width/2;this.xDegrees=180-Math.atan2(x,y)*180/Math.PI;this.updateCanvas(this.xDegrees,canvasE,false);this.updateInput(this.xDegrees,this.inputElementX,false);this.xneeds_sim_update=true;}
RotationVector.prototype.updateY=function(evt)
{if(!evt)
evt=window.event;var canvasE=this.canvasY;var x=getXRelativeToElement(canvasE,evt);var y=getYRelativeToElement(canvasE,evt);x-=canvasE.width/2;y-=canvasE.width/2;this.yDegrees=180-Math.atan2(x,y)*180/Math.PI;this.updateCanvas(this.yDegrees,canvasE,false);this.updateInput(this.yDegrees,this.inputElementY,false);this.yneeds_sim_update=true;}
RotationVector.prototype.updateZ=function(evt)
{if(!evt)
evt=window.event;var canvasE=this.canvasZ;var x=getXRelativeToElement(canvasE,evt);var y=getYRelativeToElement(canvasE,evt);x-=canvasE.width/2;y-=canvasE.width/2;this.zDegrees=180-Math.atan2(x,y)*180/Math.PI;this.updateCanvas(this.zDegrees,canvasE,false);this.updateInput(this.zDegrees,this.inputElementZ,false);this.zneeds_sim_update=true;}
RotationVector.prototype.updateInput=function(val,element,notE)
{if(element==notE)
return;element.value=val;}
RotationVector.prototype.updateXFromInputElement=function()
{var val=parseFloat(this.inputElementX.value);if(isNaN(val))
return;this.xDegrees=val;this.updateCanvas(this.xDegrees,this.canvasX,false);this.xneeds_sim_update=true;}
RotationVector.prototype.updateYFromInputElement=function()
{var val=parseFloat(this.inputElementY.value);if(isNaN(val))
return;this.yDegrees=val;this.updateCanvas(this.yDegrees,this.canvasY,false);this.yneeds_sim_update=true;}
RotationVector.prototype.updateZFromInputElement=function()
{var val=parseFloat(this.inputElementZ.value);if(isNaN(val))
return;this.zDegrees=val;this.updateCanvas(this.zDegrees,this.canvasZ,false);this.zneeds_sim_update=true;}
RotationVector.prototype.updateCanvas=function(val,canvas,notE)
{if(canvas==notE)
return;if(!canvas.getContext)
{return;}
var ctx=canvas.getContext('2d');var xsize=canvas.width;var ysize=canvas.height;var xpadding=Math.round(xsize*0.1);var diameter=xsize-(xpadding*2);var ypadding=Math.round((ysize-diameter)*0.5);drawRotationAngleCircle(ctx,xpadding,ypadding,diameter,val);}
RotationVector.prototype.updateInputs=function(notE)
{this.updateInput(this.xDegrees,this.inputElementX,notE);this.updateInput(this.yDegrees,this.inputElementY,notE);this.updateInput(this.zDegrees,this.inputElementZ,notE);this.updateCanvas(this.xDegrees,this.canvasX,notE);this.updateCanvas(this.yDegrees,this.canvasY,notE);this.updateCanvas(this.zDegrees,this.canvasZ,notE);}