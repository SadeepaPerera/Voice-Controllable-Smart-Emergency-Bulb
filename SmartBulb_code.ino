// ESP8266 Alexa Controlled RGB Smart Bulb

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"  //Include this Library
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define PIN1          D3    //LED Pin
#define NUM_LEDS1      60     //Enter number of LEDs
CRGB leds1[NUM_LEDS1];

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#define ESPALEXA_A  1SYNC
#include <Espalexa.h>

int data=255;
int r,g,b;

// prototypes
boolean connectWifi();

//callback function prototype
void colorLightChanged(uint8_t brightness, uint32_t rgb);

// Change this!!
const char* ssid = "sadeepa";      //Enter WiFi Name
const char* password = "12345Sadeepa";     //Enter WiFi Password

boolean wifiConnected = false;

Espalexa espalexa;

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection( TypicalLEDStrip );
  // Initialise wifi connection
  wifiConnected = connectWifi();
  if(wifiConnected){
    espalexa.addDevice("SmartBulb", colorLightChanged);   //Device Name

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//the color device callback function has two parameters
void colorLightChanged(uint8_t brightness, uint32_t rgb) {
  //do what you need to do here, for example control RGB LED strip
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print(", Red: ");
  Serial.print((rgb >> 16) & 0xFF); //get red component
  Serial.print(", Green: ");
  Serial.print((rgb >>  8) & 0xFF); //get green
  Serial.print(", Blue: ");
  Serial.println(rgb & 0xFF); //get blue
float hell = brightness / 255.0;
r=((rgb >> 16) & 0xFF)*hell;
g=((rgb >>  8) & 0xFF)*hell;
b=(rgb & 0xFF)*hell;
static1(r, g, b,data);

}

void static1(int r, int g, int b,int brightness)
{
  FastLED.setBrightness(brightness);
  for (int i = 0; i < NUM_LEDS1; i++ )
  {
    leds1[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 40){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
