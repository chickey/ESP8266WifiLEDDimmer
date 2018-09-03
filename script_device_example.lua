
commandArray = {}

DomDevice = 'Name in Domoticz'

IP = 'x.x.x.x'

 if devicechanged[DomDevice] then
   if(devicechanged[DomDevice]=='Off') then DomValue = 0;
   print ("Turning off " .. DomDevice);
   runcommand = "curl 'http://" .. IP .. "/led/1/" .. DomValue .. "'";
   print (runcommand);
   os.execute(runcommand);
 return commandArray
   else
    DomValue = (otherdevices_svalues[DomDevice]);
   end
   print ("Value received from Domoticz was " .. (DomValue) .." ");
   print ("Dimming "  .. (DomDevice) .. " to " .. (DomValue) .. " ");
   runcommand =  "curl 'http://" .. IP .. "/led/1/" .. DomValue .. "'";
   print (runcommand);
   os.execute(runcommand);
 end
return commandArray
