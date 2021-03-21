
#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#define SCK D1
#define DOUT D2
#include <Wire.h>
unsigned long prev;
int flag;

// WIFI ADJUST
#include <WiFiAdjust.h>

// Master Adjust
#define SLAVE_ADDR 9
bool MasterFlag = 0;
int SlavePin = D3;
int bcount;
const int leng = 165;
int CurrentSensorState[leng];
unsigned long jsd;
int trans = 0;
const int OutputNumber = 30;
const int rows = OutputNumber;
const int columns = 4;
int TimeDevice[rows][columns];
int n;

byte readI2C(int address)
{
  // Define a variable to hold byte of data
  byte bval;
  long entry = millis();
  // Read one byte at a time
  Wire.requestFrom(address, 1);
  // Wait 100 ms for data to stabilize
  while (Wire.available() == 0 && (millis() - entry) < 100)
    Serial.print("Waiting");
  // Place data into byte
  if (millis() - entry < 100)
    bval = Wire.read();
  return bval;
}

void setup()
{
  pinMode(SlavePin, INPUT);
  // Initialize SPIFFS
  WiFiSetup();
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  Serial.print("my fuck time  ");
  delay(500);
}

void loop()
{
  // To access
  if (millis() - prev > 15000)
  {
    Serial.println("Send to internet");
    flag = 1;
    prev = millis();
    for (int i = 0; i < mat; i++)
    {
      writeFile(SPIFFS, files[i], String(CurrentSensorState[i]));
    }
  }
  trans = digitalRead(SlavePin);
  // Serial.println(trans);
  if (trans == 1)
  {
    while (readI2C(SLAVE_ADDR) < 255)
    {
      // Until first byte has been received print a waiting message
      Serial.print("Waiting");
    }
    for (bcount = 0; bcount < leng; bcount++)
    {
      CurrentSensorState[bcount] = readI2C(SLAVE_ADDR);
    }
    for (int i = 0; i < leng; i++)
    {
      Serial.print(CurrentSensorState[i]);
      Serial.print(" ");
    }
    Serial.println();
    delay(200);
  }
}
