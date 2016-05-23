
var backdrops=new Array();function Backdrop(id,image,active)
{this.id=id;this.image=image;this.active=active;}
Backdrop.prototype.toTR=function()
{var tr=document.createElement("tr");var td=document.createElement("td");var a=document.createElement("a");var img=document.createElement("img");img.setAttribute("src",this.image);a.setAttribute("href","javascript:loadBackdrop("+this.id+")");a.appendChild(img);if(this.active)
setCSSClass(a,"active");else
setCSSClass(a,"inactive");td.appendChild(a);tr.appendChild(td);return tr;}
function loadBackdrop(id)
{sendRequestNonBlocking("/load?item=backdrop&value="+id);for(var i=0;i<backdrops.length;i++)
{if(backdrops[i].id==id)
backdrops[i].active=true;else
backdrops[i].active=false;}
updateBackdropUI();}
function initBackdropOptions()
{var url="/get?item=backdrops";sendRequestTo(url,true,processBackdropResponse);}
function processBackdropsDoc(doc)
{var backdropsE=doc.getElementsByTagName("backdrop");backdrops=new Array();for(var i=0;i<backdropsE.length;i++)
{var backdrop=backdropsE[i];var active=backdrop.getAttribute("active");var image=backdrop.getAttribute("image");var id=backdrop.getAttribute("id");if(!active||active=="0")
active=false;else
active=true;backdrops.push(new Backdrop(id,image,active));}
updateBackdropUI();}
function updateBackdropUI()
{var e=document.getElementById("backdrops");removeChildrenFrom(e);var tbody=document.createElement("tbody");for(var i=0;i<backdrops.length;i++)
{tbody.appendChild(backdrops[i].toTR());}
e.appendChild(tbody);}
function processBackdropResponse()
{if(this.readyState==4)
{if(this.responseXML)
{processBackdropsDoc(this.responseXML);}
else
alert("Internal problem getting XML document from server for backdrops.");}}