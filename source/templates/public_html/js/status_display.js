
function setStatusMessage(msg)
{var display=document.getElementById("status");removeChildrenFrom(display);display.appendChild(document.createTextNode(msg));}