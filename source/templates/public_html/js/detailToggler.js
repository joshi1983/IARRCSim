
function getDetailedElement(e)
{if(!e.tagName)
return false;var className=e.getAttribute("className");if(className==""||!className)
className=e.getAttribute("class");if(className=="detailed")
return e;for(var n=e.firstChild;n;n=n.nextSibling)
{var result=getDetailedElement(n);if(result)
return result;}
return false;}
function showDetails(id)
{var e=document.getElementById(id);var detailedE=getDetailedElement(e);if(detailedE)
{showElement(detailedE);}
hideOtherElements(e,detailedE);}
function hideOtherElements(e,notHideE)
{if(e==notHideE)
return;if(!e.tagName)
return;for(var n=e.firstChild;n;n=n.nextSibling)
{if(n!=notHideE)
hideElement(n);else
hideOtherElements(n,notHideE);}}
function showOtherElements(e,notShowE)
{if(e==notShowE)
return;if(!e.tagName)
return;showElement(e);for(var n=e.firstChild;n;n=n.nextSibling)
{showOtherElements(n,notShowE);}}
function showElement(e)
{if(!e.style)
return;e.style.visibility='visible';e.style.display='block';}
function hideElement(e)
{if(!e.style)
return;e.style.visibility='hidden';e.style.display='none';}
function hideDetails(id)
{var e=document.getElementById(id);var detailedE=getDetailedElement(e);if(detailedE)
{hideElement(detailedE);}
showOtherElements(e,detailedE);}