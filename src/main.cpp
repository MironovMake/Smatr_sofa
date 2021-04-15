
#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <Wire.h>
unsigned long prev;
int Internet_flag = 0;
int change;
int m;
// Master Adjust
#define SLAVE_ADDR 9
bool MasterFlag = 0;
uint8_t SlavePin = 12; // its D6
int bcount;
const int leng = 201;
int CurrentSensorState[leng];
unsigned long jsd;
bool Mega_listen = 0;
const int OutputNumber = 30;
const int rows = OutputNumber;
const int columns = 4;
int TimeDevice[rows][columns];
int n;
bool FirstTimeFlag = 0;

// WIFI ADJUST
#include <WiFiAdjust.h>

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
  // Initialize LittleFS

  WiFiSetup();
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  Serial.print("my fuck time  ");
  delay(2500);
}

void loop()
{
  //Mega_listen = digitalRead(SlavePin);
  // Serial.println(Mega_listen);
  //when Mega want to translate some date
  if (Mega_listen == 1 && FirstTimeFlag == 0)
  {
    Serial.println("I am getting date from Mega");
    // get date
    while (readI2C(SLAVE_ADDR) < 255)
    {
      Serial.print("Waiting");
    }
    for (bcount = 0; bcount < leng; bcount++)
    {
      CurrentSensorState[bcount] = readI2C(SLAVE_ADDR);
    }
    // save date to files, than post ones in internet
    for (int i = 0; i < leng; i++)
    {
      Serial.print(" ");
      Serial.print(CurrentSensorState[i]);
      SendingValueToString(i, CurrentSensorState[i]);
      events.send(String(CurrentSensorState[i]).c_str(), parametr[i], millis());
    }
    writeFile(LittleFS, MyFile, GeneralString);
    Serial.println();
    Serial.println("I success get date from slave");
    //Serial.println(GeneralString);
    //delay(50);
  }

  // send date to Mega
  if (Internet_flag == 1 || FirstTimeFlag == 1)
  {
    if (Internet_flag)
    {
      Serial.println("Something happening in internet");
      writeFile(LittleFS, MyFile, GeneralString);
    }
    if (FirstTimeFlag)
      Serial.println("I send date becouse start first time");
    //Serial.println(GeneralString);
    for (int i = 0; i < leng; i++)
    {
      // Write a charatre to the Slave
      Serial.print(" ");
      Serial.print(CurrentSensorState[i]);
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(CurrentSensorState[i]);
      Wire.endTransmission();
      //delay(50);
    }
    //Serial.print(GeneralString);
    Serial.println();
    Serial.println("I sended data to mega");
    Internet_flag = 0;
    FirstTimeFlag = 0;
  }
  //Serial.println("eher");
}
