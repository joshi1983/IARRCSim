
function getSimulatorProperty(propName)
{return sendRequestForTextTo("get?item=property&name="
+escape(propName),false);}