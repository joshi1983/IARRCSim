
function getTrackSelecter()
{var track_id="tracks";var select=document.getElementById(track_id);if(!select)
{alert("unable to get element with id '"+track_id+"'.");return false;}
return select;}
function loadSelectedTrack()
{var select=getTrackSelecter();if(!select)
return;var selectedValue=select.options[select.selectedIndex].value;var url='load?item=track&value='+escape(selectedValue);sendRequestNonBlocking(url);}
function updateTrackItems()
{var select=getTrackSelecter();if(!select)
return;var xmlDoc=getXMLDocumentFromURL("get?item=tracks&format=xml");removeChildrenFrom(select);var tracks=xmlDoc.getElementsByTagName("track");for(var i=0;i<tracks.length;i++)
{var filename=tracks[i].getAttribute("filename");if(filename!="")
{var option=document.createElement("option");option.setAttribute("value",filename);var trackName=filename;var index=trackName.lastIndexOf('.');if(index>0)
{trackName=trackName.substring(0,index);}
trackName=formatTrackFileName(trackName);option.appendChild(document.createTextNode(trackName));select.appendChild(option);}}}
function formatTrackFileName(filename)
{var result="";for(var i=0;i<filename.length;i++)
{var c=filename.charAt(i);if(c=='_')
c=' ';result=result+c;}
return result;}
function initTracks()
{updateTrackItems();}