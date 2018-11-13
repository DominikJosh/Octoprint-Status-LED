# Octoprint-Status-LED
A WS2812b based Status of Octoprint, it shows the Temperature of the bed between 30° and 60° and also the printjob status.
It polls the Ocotoprint REST API periodically via a HTTP GET request. There are some delay between the real state of the printer and the of the LED, this comes do the simple polling approch. Faster polling result in lower delay but also increases the load on the ESP and on the wifi or network in general (on Octoprint too).

Security Warning: This sketch sends unencrypted GET requests with the octoprint apikey in cleartext in the request header through your network. Although this isn't that big of a security risk though, if your octoprint is not visible to the internet (which i did not recommend anyway)


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

# The Arduino IDE Way
Download/Clone this Repository and unpack it on your favourite folder<br><br>

Download and install the latest version of the Arduino IDE for your system https://www.arduino.cc/en/Main/Software<br><br>

Add ESP Board Manager to your IDE : Therefore open File>Preferences After "Additional Boards Manager URLs" insert:<br> http://arduino.esp8266.com/stable/package_esp8266com_index.json<br><br>


Then open Tools>Board>Boards Manager Search for ESP Install latest ESP8266 Board Manager<br><br> 

Add Additional Libraries Therefore open Sketch>Include Library>Manage Libraries... Search for Adafruit Neopixel Install latest Adafruit<br> Neopixel library<br> 
Search ArduinoJson Install latest ArduinoJson	<br>
Restart Arduino IDE<br>
Open config.h and add your configuration<br>
Choose your board and com port<br>
Upload<br>
# Configuration
There are 4 main configurations in the config.h file, which must be made by user...<br><br>

Wifi SSID const char* SSID = "myWifi"; the name of your Wireless Network<br><br>

Wifi Password const char* SSID = "myWifi"; the password to your Wireless Network<br><br>

Octoprint API key const char* APIKEY = "FA9131811AF94AB48C6A9ED45AEF60FC"; like a password to the Octoprint REST Api {TBD} /doc/img/sc-octocprint-api-key.PNG You can obtain your API key in the Octoprint Webinterface in the settings under API. More info about the Octoprint API key in the API Documentation: link<br><br>

Octoprint Ip Address the Network address to your Octoprint. Same as you need to gain acces to your web interface of octoprint.<br><br>

# Example User Config:
#define LEDPIN D2 // where you Led is connected<br>
const char* SSID = "myWifi";<br>
const char* WPWD = "wifipassword";<br>
const char* APIKEY = "FA9131811AF94AB48C6A9ED45AEF60FC";<br>
String OCTOIP = "192.168.1.3";<br>
uint16_t pollInterval = 500;<br>
const uint8_t lenght = 8;<br>
int statusShow = true; 
