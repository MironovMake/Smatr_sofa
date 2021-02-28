
#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#define SCK D1
#define DOUT D2
#include <Wire.h>
int prev;
String some;
String inputParam;
int ledState = LOW; // the current state of the output pin
bool reading = 0;
int flag;
String current_procecc = "Waitting";

// WIFI ADJUST
#include <WiFiAdjust.h>

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
  Serial.print("my fuck time  ");
}

void loop()
{
  // To access
  if (millis() - prev > 60000)
  {
    flag=1;
    prev = millis();
    writeFile(SPIFFS, "/MIN.txt", String(flag));
    writeFile(SPIFFS, "/HOUR.txt", String(flag));
    writeFile(SPIFFS, "/TEMP.txt", String(flag));
  }
  if (flag)
  { // println data
    // oled_println(current_sensor, process);
    flag = 0;
    // Serial.println("flag");
  }
  MasterRequest();
}