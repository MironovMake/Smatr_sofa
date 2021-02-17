/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <Wire.h>
// encoder
#include "GyverEncoder.h"
#define CLK D5
#define DT D6
#define SW D4
Encoder enc1(CLK, DT, SW);
int previous_time;
// dallas tempereture
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D3
OneWire oneWire(ONE_WIRE_BUS);
float tempC;
DallasTemperature sensors(&oneWire);
// oled screen
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
int variable = 15;
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// RTC module
#include "RTClib.h"
RTC_DS1307 rtc;

String process[6] = {"Irrigation", "Watering", "Mixing", "Drum rotation", "Waitting", "Emergency_off"};
int current_sensor[5];
int previous_sensor[5];
/*
sensor[0] minute
sensor[1] houre
sensor[2] temperature
sensor[3] encoder
sensor[4] button
*/
bool flag;
String current_procecc = "Waitting";

// WIFI ADJUST 
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Beeline_2G_F22959";
const char* password = "dUNFcUEP";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void oled_println(int *current_sensor, String *pro)
{
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.print("temp ");
  display.print(current_sensor[2]);
  display.println("C ");
  display.println(pro[current_sensor[3]]);
  display.print(current_sensor[1]);
  display.print(":");
  display.println(current_sensor[0]);
  display.println(pro[current_sensor[4]]);
  display.display();
  Serial.println("OLED PRINT");
}
void setup()
{
Serial.begin(115200);
WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();

  

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif
  //
  enc1.setType(TYPE2);
  sensors.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    for (;;)
      ; // Don't proceed, loop forever
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void Irrigation()
{
}
void Watering()
{
}
void Mixing()
{
}
void Drum_rotation()
{
}
void Waitting()
{
}
void Emergency_off()
{
}

void loop()
{
  
  DateTime now = rtc.now();
  if (current_sensor[0] != now.minute())
  {
    current_sensor[0] = now.minute();
    current_sensor[1] = now.hour();
    sensors.requestTemperatures(); // Send the command to get temperatures
    current_sensor[2] = sensors.getTempCByIndex(0);
    Serial.println(current_sensor[0]);
    Serial.println(current_sensor[1]);
    Serial.println(current_sensor[2]);
  }

  enc1.tick();
  if (enc1.isRight()) // encoder
  {
    current_sensor[3]++;
    Serial.println(current_sensor[3]);
    if (current_sensor[3] > 5)
    {
      current_sensor[3] = 0;
    }
  }

  if (enc1.isLeft())
  {
    current_sensor[3]--;
    Serial.println(current_sensor[3]);
    if (current_sensor[3] < 1)
    {
      current_sensor[3] = 5;
    }
  }

  if (enc1.isPress()) // push button
  {
    current_sensor[4] = current_sensor[3];
    Serial.println(current_sensor[4]);
  }

  for (int i = 0; i < 5; i++) // check if data changed
  {
    if (current_sensor[i] != previous_sensor[i])
    {
      previous_sensor[i]=current_sensor[i];
      Serial.println(current_sensor[i]);
      flag = 1;
      
    }
  }

  if (flag)
  { // println data
    oled_println(current_sensor, process);
    flag = 0;
    Serial.println("flag");
  }
}
