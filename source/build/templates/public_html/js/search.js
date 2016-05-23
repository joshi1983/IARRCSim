
var searchcache="";var pages=new Array();var timer=false;function updateSearchCache()
{sendRequestTo("cache.txt",true,handleSearchCacheResponse);}
function handleSearchCacheResponse()
{if(this.readyState==4)
{searchcache=this.responseText;parsePages();}}
function parsePages()
{var seperator="##-#$%!@#$%^&*()webpage-path: ";var pcontents=searchcache.split(seperator);pages=new Array();for(var i=0;i<pcontents.length;i++)
{var s=pcontents[i];if(s=="")
continue;var index=s.indexOf('\n');if(index>0)
{var url=s.substring(0,index);s=s.substring(index+1);index=s.indexOf('\n');if(index>0)
{var title=s.substring(0,index);s=s.substring(index+1);index=s.indexOf('\n');if(index>0)
{var description=s.substring(0,index);s=s.substring(index+1);var contents=s.substring(index+1);pages.push(new Page(url,title,description,contents));}
else
alert("cache.txt problem.  Missing description.");}
else
alert("cache.txt problem.  Missing title.");}}}
function getSearchResults(searchString)
{var result=new Array();for(var i=0;i<pages.length;i++)
{pages[i].rememberValue(searchString);if(pages[i].searchValue>0)
result.push(pages[i]);}
var sorted=false;while(!sorted)
{sorted=true;for(var i=0;i<result.length-1;i++)
{if(result[i].searchValue<result[i+1].searchValue)
{var temp=result[i];result[i]=result[i+1];result[i+1]=temp;sorted=false;}}}
return result;}
function delayedShowSearchResults()
{if(timer)
{clearTimeout(timer);}
timer=setTimeout("listResults()",100);}
function listResults(searchstring)
{if(!searchstring)
{var input=document.getElementById("searchstring").value;searchstring=input;}
var id="searchresults";var display=document.getElementById(id);if(!display)
{alert("Unable to find element with id: "+id);return;}
removeChildrenFrom(display);searchstring=singleSpaces(searchstring);var pages=getSearchResults(searchstring);for(var i=0;i<pages.length;i++)
{display.appendChild(pages[i].getAsSearchResultElement());}
timer=false;}
function singleSpaces(searchString)
{var result="";var inSpaces=true;for(var i=0;i<searchString.length;i++)
{var c=searchString.charAt(i);if(c==' '||c=='\t'||c=='\r'||c=='\n')
c=' ';if(c==' ')
{if(!inSpaces)
result+=c;inSpaces=true;}
else
{result+=c;inSpaces=false;}}
return result;}
function Page(url,title,description,contents)
{this.url=url;this.title=title;this.description=description;this.contents=contents;}
Page.prototype.rememberValue=function(searchString)
{this.searchValue=this.getSearchValue(searchString);}
Page.prototype.getSearchValue=function(searchString)
{var numReps=this.contents.split(searchString).length;var result=0;result+=numReps*100000-100000;this.searchMatch=false;if(result>0)
{var len=50;var index=this.contents.indexOf(searchString);if(index-len<0)
{len=index;}
index-=len;var offset=0;if(index<0)
{offset=-index;index=0;}
this.searchMatch=document.createElement("p");this.searchMatch.appendChild(document.createTextNode("..."+this.contents.substring(index,index+len)));var b=document.createElement("b");b.appendChild(document.createTextNode(searchString));this.searchMatch.appendChild(b);this.searchMatch.appendChild(document.createTextNode(this.contents.substring(index+offset+len+searchString.length,index+searchString.length+50+len)+"..."));}
var contents=this.contents.toLowerCase();searchString=searchString.toLowerCase();result+=contents.split(searchString).length*5000-5000;var searchTokens=searchString.split(' ');for(var i=0;i<searchTokens.length;i++)
{result+=contents.split(searchTokens[i]).length*100-100;}
return result;}
Page.prototype.getHyperlink=function()
{var a=document.createElement("a");a.setAttribute("href",this.url);var text=this.title;if(text.length<2)
text=this.url;a.appendChild(document.createTextNode(text));return a;}
Page.prototype.getAsSearchResultElement=function()
{var e=document.createElement("div");var h3=this.getHyperlink();var description=document.createElement("p");description.appendChild(document.createTextNode(this.description));setCSSClass(e,"searchResult");e.appendChild(h3);e.appendChild(description);if(this.searchMatch)
e.appendChild(this.searchMatch);return e;}