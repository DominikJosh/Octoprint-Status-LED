# Octoprint-Status-LED
A WS2812b based Status of Octoprint, it shows the Temperature of the bed between 30° and 60° and also the printjob status.
It polls the Ocotoprint REST API periodically via a HTTP GET request. There are some delay between the real state of the printer and the of the LED, this comes do the simple polling approch. Faster polling result in lower delay but also increases the load on the ESP and on the wifi or network in general (on Octoprint too).

Security Warning: This sketch sends unencrypted GET requests with the octoprint apikey in cleartext in the request header through your network. Although this isn't that big of a security risk though, if your octoprint is not visible to the internet (which i did not recommend anyway)

# Instructions:
You have to set up the config.h file to your Routers SSID, set up your WLAN-Passwort, set in your Octoprint http-adress, IP and API, then set your data-pin for the WS2812b Din and
select your number of LED´s. 
Set the statusShow to false if you didn´t want to show a rainbowcycle when the printer is ready and the status of the printjob. This is
usefull, when you put the strip as a light for your printer. So is statusShow = false, it shows by printer ready and while printing only a
powerfull white light.

# Description:
The color of the lamp will show you the status as follows:

-fade yellow: booting / connecting<br>
-rainbowcycle: Printer is ready<br>
  -if false: white light
-blue to red: Printer is heating the bed shows every 5°C from 30-60°C<br>
-blue to green: Printer is printing, shows progress in 25%<br>
  -if false: white light
-fade blue: Print is paused<br>
-fade red: error occurred<br>
-flashing white: HTTP Code Unauthorized<br>
-fade green: HTTP Code Conflict<br><br>

It will also output small informations on the serial line.<br>

# Hardware
Electronics<br>
BOM:<br>

1x Wemos d1 mini (or any esp8266 with 4Mb flash)<br>
1x WS2812b RGB Led<br>
1x Wires (3 cores, about 3-4cm)<br>
1x Micro USB Cable and 5V Power supply <br>
