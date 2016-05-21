
function resetAllPopupAnchors()
{hideAllDivPopups();var e=document.getElementById("extrasAnchor");e.setAttribute("href","javascript:showPopup('popup_extras','extrasAnchor')");var popup=document.getElementById("popup_extras");popup.style.left=getClientX(e);}
function showPopup(id,anchorID)
{resetAllPopupAnchors();var e=document.getElementById(id);e.style.visibility='visible';e.style.display='block';var a=document.getElementById(anchorID);a.setAttribute('href',"javascript:resetAllPopupAnchors()");}
function hideAllDivPopups()
{var divs=document.getElementsByTagName("div");for(var i=0;i<divs.length;i++)
{var div=divs[i];var id=div.getAttribute("id");if(!id)
continue;if(id.length<6)
continue;var prefix=id.substring(0,6);if(prefix=="popup_")
{div.style.visibility='hidden';div.style.display='none';}}}