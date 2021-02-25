
#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#define SCK D1
#define DOUT D2
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

// RTC module
int Watering_time;
String process[6] = {"Irrigation", "Watering", "Mixing", "Drum rotation", "Waitting", "Emergency_off"};
int current_sensor[5];
int previous_sensor[5];
String some;
String inputParam;
int ledState = LOW; // the current state of the output pin
bool reading = 0;
/*
sensor[0] minute
sensor[1] houre
sensor[2] temperature
sensor[3] encoder
sensor[4] button
*/
int flag;
String current_procecc = "Waitting";

// WIFI ADJUST
#include <WiFiAdjust.h>
/*
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
*/

void MasterRequest()
{
  Wire.beginTransmission(8);   /* Начинаем передачу на адресе 8 */
  Wire.write("Hello Arduino"); /* Отправляем "hello Arduino" */
  Wire.endTransmission();      /* прекращаем передачу */

  Wire.requestFrom(8, 13); /* запрашиваем и считываем данные с 8 и 13 адреса slave устройства */
  while (Wire.available())
  {
    int c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  delay(1000);
}

void setup()
{
  Serial.begin(9600);
  // Initialize SPIFFS
  WiFiSetup();
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  enc1.setType(TYPE2);
  delay(500);
  delay(500);
  sensors.begin();
  Serial.print("my fuck time  ");
}

void loop()
{
  // To access
  if (millis() - current_sensor[0] > 60000)
  {
    current_sensor[0] = millis();
    current_sensor[1] = 2;
    sensors.requestTemperatures(); // Send the command to get temperatures
    current_sensor[2] = sensors.getTempCByIndex(0);
    writeFile(SPIFFS, "/MIN.txt", String(current_sensor[0]));
    writeFile(SPIFFS, "/HOUR.txt", String(current_sensor[1]));
    writeFile(SPIFFS, "/TEMP.txt", String(current_sensor[2]));
  }

  enc1.tick();
  if (enc1.isRight()) // encoder
  {
    current_sensor[3]++;
    if (current_sensor[3] > 5)
    {
      current_sensor[3] = 0;
    }
    writeFile(SPIFFS, "/ENCODER.txt", String(current_sensor[3]));
  }
  if (enc1.isLeft())
  {
    current_sensor[3]--;
    if (current_sensor[3] < 1)
    {
      current_sensor[3] = 5;
    }
    writeFile(SPIFFS, "/ENCODER.txt", String(current_sensor[3]));
    //writeFile(SPIFFS, "/ENCODER.txt", "suck");
  }
  if (enc1.isPress()) // push button
  {
    current_sensor[4] = current_sensor[3];
  }
  for (int i = 0; i < 5; i++) // check if data changed
  {
    if (current_sensor[i] != previous_sensor[i])
    {
      previous_sensor[i] = current_sensor[i];
      Serial.println(current_sensor[i]);
      flag = 1;
    }
  }

  if (flag)
  { // println data
    // oled_println(current_sensor, process);
    flag = 0;
    // Serial.println("flag");
  }
  MasterRequest();
}