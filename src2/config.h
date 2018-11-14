
// User config
// more info in the README.md in the root of the repository

#ifndef CONFIG  // not #ifnotdef
#define CONFIG

  #define PIN D2
  const char* SSID = ""; // wlan SSID
  const char* WPWD = ""; // wlan password
  const char* APIKEY = ""; // octoprint api key (optain it in the web interface)
  String OCTOIP = ""; // the ip address of octoprint
  uint16_t pollInterval = 500; // ms, lower value will increase poll rate, but the animation time will limit the poll rate too
  const uint8_t length = 8; // number of LEDs, minimum amount of LEDs is 4 better ist 8 supported: 4,8,12,16,20,24,28,32,36,40...120)
  int statusShow = true;   //set false for white light while printing 
  int readyShow = true;    //set false for white light while printer is ready

#endif
