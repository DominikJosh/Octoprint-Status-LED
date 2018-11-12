# Octoprint-Status-LED
A WS2812b based Status of Octoprint, it shows the Temperature of the bed between 30° and 60° and also the printjob status

You have to set up the config.h file to your Routers SSID, set up your WLAN-Passwort, set in your Octoprint http-adress, IP and API, then set your data-pin for the WS2812b Din and
select your number of LED´s. 
Set the statusShow to false if you didn´t want to show a rainbowcycle when the printer is ready and the status of the printjob. This is
usefull, when you put the strip as a light for your printer. So is statusShow = false, it shows by printer ready and while printing only a
powerfull white light.
