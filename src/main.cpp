#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
// encoder
#include "GyverEncoder.h"
#define CLK D5
#define DT D6
#define SW D4
int counter;
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
String current_procecc="Waitting";
void oled_print(int var, String count, int one, int two, String current_procec)
{
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.print("temp ");
  display.print(var);
  display.println("C ");
  display.println(count);
  display.print(one);
  display.print(":");
  display.println(two);
  display.println(current_procec);
  display.display();
}
void setup()
{
  Serial.begin(115200);

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

void Irrigation(){

}
void Watering(){
  
}
void Mixing(){
  
}
void Drum_rotation(){
  
}
void Waitting(){
  
}
void Emergency_off(){
  
}

void loop()
{
  DateTime now = rtc.now();

  if (previous_time!=now.minute())
  {
    sensors.requestTemperatures(); // Send the command to get temperatures
    tempC = sensors.getTempCByIndex(0);
    oled_print(tempC, process[counter], now.hour(), now.minute(),current_procecc);
    previous_time = now.minute();
  }
  enc1.tick();
  if (enc1.isRight()) // если был поворот
  {
    counter++;
    if (counter > 5)
    {
      counter = 0;
    }
    oled_print(tempC, process[counter], now.hour(), now.minute(),current_procecc);
  }
  if (enc1.isLeft())
  {
    counter--;
    if (counter < 1)
    {
      counter = 5;
    }
    oled_print(tempC, process[counter], now.hour(), now.minute(),current_procecc);
  }
  if (enc1.isPress()){
current_procecc=process[counter];
oled_print(tempC, process[counter], now.hour(), now.minute(),current_procecc);
  }   
}
