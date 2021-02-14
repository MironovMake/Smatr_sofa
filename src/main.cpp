#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D3
#define CLK D5
#define DT D6
#define SW D4
#include "GyverEncoder.h"
int counter;
//Encoder enc1(CLK, DT);      // для работы без кнопки
Encoder enc1(CLK, DT, SW);
int previous_time;
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
int variable = 15;

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

#define XPOS 0 // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2
float tempC;
void testdrawstyles(int var, int count)
{
  display.clearDisplay();
  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.print(var);
  display.println(" C");
    display.setTextSize(1);              // Normal 1:1 pixel scale
  display.print("counter ");
  display.println(count);
  display.display();
}

void setup()
{
  Serial.begin(115200);
  enc1.setType(TYPE2);
  sensors.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  delay(1000); // Pause for 2 seconds
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(1000);
  testdrawstyles(variable, counter); // Draw 'stylized' characters
}

void loop()
{
  if(millis()-previous_time>3000){
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC = sensors.getTempCByIndex(0);
  previous_time=millis();
  }
  
  enc1.tick();
  if (enc1.isRight())
  {
    counter++;
    testdrawstyles(tempC, counter);
  } // если был поворот
  if (enc1.isLeft())
  {
    counter--;
    testdrawstyles(tempC, counter);
  }

}