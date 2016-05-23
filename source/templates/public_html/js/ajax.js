
function setAJAXMessage(msg)
{var e=document.getElementById("ajax_message");if(!e)
{alert(msg);return;}
removeChildrenFrom(e);e.appendChild(document.createTextNode(msg));}
function showAJAXNotSupported()
{var msg="Enable XMLHttpRequest object to take full advantage of this website.";setAJAXMessage(msg);}
function getXMLHTTPRequestObject()
{var xmlhttp;if(window.XMLHttpRequest)
{xmlhttp=new XMLHttpRequest();}
else
{xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");}
if(!xmlhttp)
{showAJAXNotSupported();}
return xmlhttp;}
function getXMLDocumentFromURL(url)
{var xmlhttp=getXMLHTTPRequestObject();xmlhttp.open("GET",url,false);xmlhttp.send();var xmlDoc=xmlhttp.responseXML;return xmlDoc;}
function sendRequestTo(url,listening,callbackfunction)
{var xmlhttp=getXMLHTTPRequestObject();if(!xmlhttp)
{alert("Internal problem.  Unable to make xmlhttp request.");return;}
if(listening)
{if(!callbackfunction)
{alert("Internal problem.  "
+"sendRequestTo called for listening but callbackfunction unspecified.");}
else
{if(xmlhttp.attachEvent)
xmlhttp.attachEvent("onreadystatechange",callbackfunction);else
xmlhttp.onreadystatechange=callbackfunction;}}
xmlhttp.open("GET",url,listening);xmlhttp.send();return xmlhttp.responseXML;}
function sendRequestForTextTo(url,listening,callbackfunction)
{var xmlhttp=getXMLHTTPRequestObject();if(listening)
xmlhttp.onreadystatechange=callbackfunction;xmlhttp.open("GET",url,listening);xmlhttp.send();return xmlhttp.responseText;}
function sendRequestNonBlocking(url)
{sendRequestTo(url,true,new Function());}
function sendRequestBlocking(url)
{sendRequestTo(url,false);}
function setINIProperty(propertyName,value)
{var url="set?item=iniproperty&name="+propertyName+"&value="+value;sendRequestNonBlocking(url);}
function removeChildrenFrom(e)
{while(e.firstChild)
{e.removeChild(e.firstChild);}}
function setCSSClass(e,className)
{if(e.setAttribute)
{e.setAttribute("className",className);e.setAttribute("class",className);}}
function getElementWidth(e)
{if(e.clientWidth)
return e.clientWidth;else if(e.width)
return e.width;else if(e.offsetWidth)
return e.offsetWidth;else
return 1;}
function getElementHeight(e)
{if(e.clientHeight)
return e.clientHeight;else if(e.height)
return e.height;else if(e.offsetHeight)
return e.offsetHeight;else
return 1;}
function getScrollTop()
{var ScrollTop=document.body.scrollTop;if(ScrollTop==0)
{if(window.pageYOffset)
ScrollTop=window.pageYOffset;else
ScrollTop=(document.body.parentElement)?document.body.parentElement.scrollTop:0;}
return ScrollTop;}
function getScrollLeft()
{var ScrollLeft=document.body.scrollLeft;if(ScrollLeft==0)
{if(window.pageYOffset)
ScrollLeft=window.pageXOffset;else
ScrollLeft=(document.body.parentElement)?document.body.parentElement.scrollLeft:0;}
return ScrollLeft;}
function getClientY(oElement)
{var iReturnValue=0;while(oElement!=null)
{iReturnValue+=oElement.offsetTop;oElement=oElement.offsetParent;}
return iReturnValue;}
function getClientX(oElement)
{var iReturnValue=0;while(oElement!=null)
{iReturnValue+=oElement.offsetLeft;oElement=oElement.offsetParent;}
return iReturnValue;}
function getXRelativeToElement(e,evt)
{return evt.clientX-(getClientX(e)-getScrollLeft());}
function getYRelativeToElement(e,evt)
{var y=getClientY(e)-getScrollTop();return evt.clientY-y;}
function getFiringObject(evt)
{if(evt.srcElement)
return evt.srcElement;else
{return evt.target;}}
function getAncestorWithTagName(e,tagName)
{if(!e)
return false;if(e.tagName.toLowerCase()==tagName.toLowerCase())
return e;else
return getAncestorWithTagName(e.parentNode,tagName);}