#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>

#include "config.h"

#define USE_SERIAL Serial

uint8_t wiCount = 0;
uint32_t previousMillis = 0;

//Animation Variables
//
uint8_t k = 0, stage = 0, modus = 6;
uint32_t last = 0;
//
//end Animaton Variables

const char *prefix = "http://";
const char *postfix ="/api/printer?exclude=sd";
const char *postfix2 ="/api/job";

String url = prefix +OCTOIP+ postfix;
const char *URL = url.c_str();
String url2 = prefix +OCTOIP+ postfix2;
const char *URL2 = url2.c_str();

int bedTemp = 0;
int progress = 0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int n = 0;
int e = 0;
int f = 0;
int g = 0;
int h = 0;
int m = 0;

ESP8266WiFiMulti WiFiMulti;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println("try connecting to Wifi");

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(SSID, WPWD);
    delay(100);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= pollInterval) {
      previousMillis = currentMillis;

      // wait for WiFi connection
      if(WiFiMulti.run() == WL_CONNECTED) {

          HTTPClient http;

          http.begin(URL); //HTTP

          //USE_SERIAL.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          http.addHeader("X-Api-Key", APIKEY);
          int httpCode = http.GET();

          // httpCode will be negative on error
          if(httpCode > 0) {
              // HTTP header has been send and Server response header has been handled
              USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

              String payload = http.getString();
              USE_SERIAL.println(payload);

              if(httpCode == HTTP_CODE_OK) {
                // 200 get state flags..
                // Allocate JsonBuffer
                // Use arduinojson.org/assistant to compute the capacity.
                // {
                //   "state": {
                //     "flags": {
                //       "closedOrError": false,
                //       "error": false,
                //       "operational": true,
                //       "paused": false,
                //       "printing": false,
                //       "ready": true,
                //       "sdReady": false
                //     },
                //     "text": "Operational"
                //   }
                // }
                const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(7);
                DynamicJsonBuffer jsonBuffer(capacity);

                // Parse JSON object
                JsonObject& root = jsonBuffer.parseObject(payload);
                if (!root.success()) {
                  Serial.println(F("Parsing failed!"));
                  return;
                }
                bedTemp = (root["temperature"]["bed"]["actual"]);
                
                if(root["state"]["flags"]["printing"] && statusShow ==true)
                {
                  modus = 3; // Yellow
                }
                else if(root["state"]["flags"]["printing"] && statusShow ==false)
                {
                  modus = 8; // White light
                }
                else if(root["state"]["flags"]["paused"])
                {
                  modus = 2; // blue
                  // TBD: should display rainbow now and on
                }
                else if(root["state"]["flags"]["error"])
                {
                  modus = 0; // red
                }
                // finsihed flag does not exist :(
                // else if(root["state"]["flags"]["finished"])
                // {

                // }
                else if(root["state"]["flags"]["ready"] && bedTemp <28 && readyShow ==true)
                {
                  modus = 4;
                }
                else if(root["state"]["flags"]["ready"] && bedTemp <28 && readyShow ==false)
                {
                  modus = 10;
                }            
                
              else if(root["state"]["flags"]["ready"] && bedTemp >=28 &&bedTemp <85)
              {
                modus = 7;
              }
              else if(root["state"]["flags"]["ready"] && bedTemp >=85)
              {
                modus = 9;
              }
              else if(httpCode == HTTP_CODE_UNAUTHORIZED)
              {
                  // wrong api key
                  modus = 5; // Red
                  // TBD: should stop after 2min
              }
              else if(httpCode == HTTP_CODE_CONFLICT)
              {
                // Printer is not operational
                modus = 1; // Green
              }
            }
            else
            {
                USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
                
http.begin(URL2); //HTTP
 
          //USE_SERIAL.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          http.addHeader("X-Api-Key", APIKEY);
          int httpCode = http.GET();
 
          // httpCode will be negative on error
          if(httpCode > 0) {
              // HTTP header has been send and Server response header has been handled
              USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
 
              String payload = http.getString();
              USE_SERIAL.println(payload);
 
              if(httpCode == HTTP_CODE_OK) {
                // 200 get state flags..
                // Allocate JsonBuffer
                // Use arduinojson.org/assistant to compute the capacity.
                // {
                //   "state": {
                //     "flags": {
                //       "closedOrError": false,
                //       "error": false,
                //       "operational": true,
                //       "paused": false,
                //       "printing": false,
                //       "ready": true,
                //       "sdReady": false
                //     },
                //     "text": "Operational"
                //   }
                // }
                const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(7);
                DynamicJsonBuffer jsonBuffer(capacity);
 
                // Parse JSON object
                JsonObject& root = jsonBuffer.parseObject(payload);
                if (!root.success()) {
                  Serial.println(F("Parsing failed!"));
                  return;
                }
 
                progress = (root["progress"]["completion"]);
                
                
                

          http.end();
      }


      
      else if(WiFiMulti.run() == WL_CONNECT_FAILED)
      {
        USE_SERIAL.println("");
        USE_SERIAL.println("WL_CONNECT_FAILED");
        USE_SERIAL.println("Please Check Password");
      }
      else
      {
        USE_SERIAL.print(".");
        wiCount++;
        if(wiCount == 20)
        {
          USE_SERIAL.println("");
          USE_SERIAL.println("Please Check SSID");
          wiCount = 0;
        }
        //USE_SERIAL.println(WiFiMulti.run());
      }

    }
          }
          }// interval

 switch (modus) {
   case 0: {
     fade_red(&last, &k, &stage, length);
     break;
   }
   case 1: {
     fade_green(&last, &k, &stage, length);
      break;
    }

   case 2: {
      fade_blue(&last, &k, &stage, length);
      break;
    }
  case 3: {
    prozent(5);
    break;
   }
  case 4: {
      rainbowCycle(5);
      break;
    }
  case 5: {
      flash(&last, &stage, length);
      break;
    }
  case 6: {
      fade_yellow(&last, &k, &stage, length);
      break;
    }
  case 7: {
      tempShow(5);
      break;
    }
  case 8: {
      noneStatus(5);
      break;
    }
  case 9: {
      fire(&last, &k, &stage, length);
      break;
    }
  case 10: {
      noneReady(5);
      break;
    }
  default: {

      break;
    }
  }//switch

}
}//loop



//Animationfunctions
//
void noneStatus (uint8_t wait) {
  for(int i = 0; i < length; i++) {
      strip.setPixelColor(i, 0 + statusRed, 0 + statusGreen, 0 + statusBlue);
    }
    strip.show();
    delay(wait);
}

void noneReady (uint8_t wait) {
  for(int i = 0; i < length; i++) {
      strip.setPixelColor(i, 0 + readyRed, 0 + readyGreen, 0 + readyBlue);
    }
    strip.show();
    delay(wait);
}


void prozent (uint8_t wait) {
    f=length-g;
    g=e*h;
    if (length ==4 || length ==12 || length ==20 || length ==28 || length ==44 || length ==52 || length ==60 || length ==68 || length ==76 || length ==84 || length ==92 || length ==100 || length ==108 || length ==116){
    e=length/4;
    if (progress >=0 && progress <25)
    {
    h=0;
      }
    if (progress >=25 && progress <50)
    {
    h=1;
      }
    else if (progress >=50 && progress <75)
    {
      h=2;
    }
    else if (progress >=75 && progress <99)
    {
      h=3;
      }    
    else if (progress >=99 && progress <100)
    {
      h=4;
      }         
    for(m=0;m<=g;m++)
    {
      strip.setPixelColor(m, 0, 255, 0);        
    }
    for(m=g;m<=length;m++)
    {
      strip.setPixelColor(m, 0, 0, 255);
    }
    }

    else if (length ==8  || length ==16  || length ==24  || length ==32  || length ==40  || length ==48  || length ==56  || length ==64  || length ==72  || length ==80  || length ==88  || length ==96  || length ==104  || length ==112  || length ==120){
    e=length/8;
    if (progress >=0 && progress <12)
    {
    h=0;
      }
    if (progress >=12 && progress <25)
    {
    h=1;
      }
    else if (progress >=25 && progress <37)
    {
      h=2;
    }
    else if (progress >=37 && progress <50)
    {
      h=3;
      }    
    else if (progress >=50 && progress <62)
    {
      h=4;
      } 
    if (progress >=62 && progress <75)
    {
    h=5;
      }
    else if (progress >=75 && progress <87)
    {
      h=6;
    }
    else if (progress >=87 && progress <99)
    {
      h=7;
      }    
    else if (progress >=99 && progress <100)
    {
      h=8;
      }          
    for(m=0;m<=g;m++)
    {
      strip.setPixelColor(m, 0, 255, 0);        
    }
    for(m=g;m<=length;m++)
    {
      strip.setPixelColor(m, 0, 0, 255);
    }
    }
    strip.show();
     delay(wait);
  }
    


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
     } 
    }


void tempShow (uint8_t wait) {
    b=length-c;
    c=a*d;
    if (length ==8  || length ==16  || length ==24  || length ==32  || length ==40  || length ==48  || length ==56  || length ==64  || length ==72  || length ==80  || length ==88  || length ==96  || length ==104  || length ==112  || length ==120){
    a=length/8;
    if (bedTemp >=28 && bedTemp <30)
    {
    d=1;
      }
    else if (bedTemp >=30 && bedTemp <35)
    {
    d=2;
      }  
    else if (bedTemp >=35 && bedTemp <40)
    {
      d=3;
    }
    else if (bedTemp >=40 && bedTemp <45)
    {
      d=4;
      }    
    else if (bedTemp >=45 && bedTemp <50)
    {
      d=5;
      }        
    else if (bedTemp >=50 && bedTemp <55)
    {
      d=6;
      }
    else if (bedTemp >=55 && bedTemp <60)
    {
      d=7;
      }
    else if (bedTemp >=60)
    {
      d=8;
      }
    }
    else if (length ==4 || length ==12 || length ==20 || length ==28 || length ==44 || length ==52 || length ==60 || length ==68 || length ==76 || length ==84 || length ==92 || length ==100 || length ==108 || length ==116){
    a=length/4;
    if (bedTemp >=28 && bedTemp <35)
    {
    d=1;
      }
    else if (bedTemp >=40 && bedTemp <45)
    {
      d=2;
      }          
    else if (bedTemp >=50 && bedTemp <55)
    {
      d=3;
      }    
    else if (bedTemp >=60)
    {
      d=4;
      }
    }
    for(n=0;n<=c;n++)
    {
      strip.setPixelColor(n, 255, 0, 0);        
    }
    for(n=c;n<=length;n++)
    {
      strip.setPixelColor(n, 0, 0, 255);
    }
    strip.show();
     delay(wait);
  }
  
void fire(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {
unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 400 ) {
    *lastmillis = millis();
    if (*stage == 0) {
      for (int i = 0; i < length; i++) {
        strip.setPixelColor(i, 255, 25, 0);
      }
      strip.show();
      *stage = 1;
    }
    else {
      if (*stage == 1) {
        for (int i = 0; i < length; i++) {
          strip.setPixelColor(i, 255, 45, 0);
        }
        strip.show();
        *stage = 0;
      }
    }
  }
}

void rainbow(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 200) {
    *lastmillis = millis();
    switch (*stage) {
      case 0: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 255 - *k, 0, *k);
          }
          break;
        }

      case 1: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, *k, 255 - *k);
          }
          break;
        }

      case 2: {
          for (int i = 0; i < length; i++) {
            strip.setPixelColor(i, *k, 255 - *k, 0);
          }
          break;
        }
    }
    strip.show();
    if (*k == 254) {
      *stage = (*stage + 1) % 3;
    }
    *k = (*k + 1) % 255;
  }
}


void fade_red(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 20) {
    *lastmillis = millis();
    switch (*stage) {
      case 0: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 63 + *k, 0);
          }
          break;
        }

      case 1: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 255 - *k, 0);
          }
          break;
        }

    }
    strip.show();
    if (*k == 191) {
      *stage = (*stage + 1) % 2;
    }
    *k = (*k + 1) % 192;
  }
}

void fade_green(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 20) {
    *lastmillis = millis();
    switch (*stage) {
      case 0: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 63 + *k, 0);
          }
          break;
        }

      case 1: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 255 - *k, 0);
          }
          break;
        }

    }
    strip.show();
    if (*k == 191) {
      *stage = (*stage + 1) % 2;
    }
    *k = (*k + 1) % 192;
  }
}

void fade_blue(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 20) {
    *lastmillis = millis();
    switch (*stage) {
      case 0: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 0, 63 + *k);
          }
          break;
        }

      case 1: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 0, 0, 255 - *k);
          }
          break;
        }

    }
    strip.show();
    if (*k == 191) {
      *stage = (*stage + 1) % 2;
    }
    *k = (*k + 1) % 192;
  }
}

void fade_yellow(uint32_t* lastmillis, uint8_t* k, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 20) {
    *lastmillis = millis();
    switch (*stage) {
      case 0: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 63 + *k, 63 + *k, 0);
          }
          break;
        }

      case 1: {
          for (int i = 0; i < length; i++)  {
            strip.setPixelColor(i, 255 - *k, 255 - *k, 0);
          }
          break;
        }

    }
    strip.show();
    if (*k == 191) {
      *stage = (*stage + 1) % 2;
    }
    *k = (*k + 1) % 192;
  }
}

void police(uint32_t* lastmillis, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 400 ) {
    *lastmillis = millis();
    if (*stage == 0) {
      for (int i = 0; i < length; i++) {
        strip.setPixelColor(i, 255, 0, 0);
      }
      strip.show();
      *stage = 1;
    }
    else {
      if (*stage == 1) {
        for (int i = 0; i < length; i++) {
          strip.setPixelColor(i, 0, 0, 255);
        }
        strip.show();
        *stage = 0;
      }
    }
  }
}


void flash(uint32_t* lastmillis, uint8_t* stage, uint8_t length) {

  unsigned long  currentmillis = millis();

  if ((currentmillis - *lastmillis) >= 40 ) {
    *lastmillis = millis();
    if (*stage == 0) {
      for (int i = 0; i < length; i++) {
        strip.setPixelColor(i, 255, 255, 255);
      }
      strip.show();
      *stage = 1;
    }
    else {
      if (*stage == 1) {
        for (int i = 0; i < length; i++) {
          strip.setPixelColor(i, 0, 0, 0);
        }
        strip.show();
        *stage = 0;
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
//
//End Animationfunctions
