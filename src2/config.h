
// User config
// more info in the README.md in the root of the repository

#ifndef CONFIG  // not #ifnotdef
#define CONFIG

  #define PIN D2
  const char* SSID = "FRITZ!Box 6360 Cable"; // wlan SSID
  const char* WPWD = "9884406786530457"; // wlan password
  const char* APIKEY = "C0A147736CF443AA8078CEEC4C1BE53B"; // octoprint api key (optain it in the web interface)
  String OCTOIP = "192.168.178.48"; // the ip address of octoprint
  uint16_t pollInterval = 500; // ms, lower value will increase poll rate, but the animation time will limit the poll rate too
  const uint8_t length = 7; // number of LEDs
  int statusShow = true;   //set false for white light while printing and while printer is ready

#endif
