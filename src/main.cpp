
#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <Wire.h>
unsigned long prev;
int Internet_flag = 0;
int change;
int m;
// Master Adjust
#define SLAVE_ADDR 11
bool MasterFlag = 0;
uint8_t SlavePin = 12; // its D6
int bcount;
const int leng = 183;
int CurrentSensorState[leng];
int PreviousSensorState[leng];
int InternetSensorState[leng];
unsigned long OutOffHere;
bool Mega_listen = 0;
int n;
bool FirstTimeFlag = 1;
byte bval;
unsigned long frecu;
unsigned long LastTimeGeting;
// WIFI ADJUSTs
#include <WiFiAdjust.h>
String adr;
byte readI2C(int address)
{
  // Define a variable to hold byte of data
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
  Serial.begin(115200);
  Serial.println("ESP8266 INIT");
  Serial.println(F("Inizializing FS..."));
  pinMode(SlavePin, INPUT);
  // Initialize LittleFS
  WiFiSetup();
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.localIP());

  adr = WiFi.localIP().toString();
  //передача IP адреса при включении
  for (int i = 0; i < adr.length(); i++)
  {
    Wire.beginTransmission(SLAVE_ADDR);
    bval = adr[i];
    Wire.write(bval);
    Wire.endTransmission();
  }
  Wire.beginTransmission(SLAVE_ADDR);
  CurrentSensorState[leng - 1] = 254;
  Wire.write(CurrentSensorState[leng - 1]);
  Wire.endTransmission();
}

void loop()
{
  Mega_listen = digitalRead(SlavePin);
  if (Mega_listen == 1 && millis() - frecu > 1000)
  {
    frecu = millis();
    Serial.println("I am getting date from Mega");
    // get date
    OutOffHere = millis();
    while (readI2C(SLAVE_ADDR) < 255) //&& (millis() - OutOffHere) < 100
    {
      Serial.print("STUCK");
    }
    for (bcount = 0; bcount < leng + 1; bcount++)
    {
      CurrentSensorState[bcount] = readI2C(SLAVE_ADDR);
      if (CurrentSensorState[bcount] == 255)
        bcount--;
      Serial.print(CurrentSensorState[bcount]);
      Serial.print(" ");
    }

    // save date to files, than post ones in internet
    for (int i = 0; i < leng; i++)
    {
      Serial.print(" ");
      Serial.print(CurrentSensorState[i]);
      if (CurrentSensorState[i] != PreviousSensorState[i] && i != 174 && i != 175)
      {
        SendingValueToString(i, CurrentSensorState[i]);
        events.send(String(CurrentSensorState[i]).c_str(), parametr[i], millis());
        PreviousSensorState[i] = CurrentSensorState[i];
      }
      if (CurrentSensorState[i] != PreviousSensorState[i] && (i == 174 || i == 175))
      {
        SendingValueToString(i, CurrentSensorState[i]);
        events.send(String(CurrentSensorState[174] * 24 + CurrentSensorState[175]).c_str(), parametr[172], millis());
        PreviousSensorState[i] = CurrentSensorState[i];
      }
      LastTimeGeting = millis();
    }

    writeFile(LittleFS, MyFile, GeneralString);
    Serial.println();
    Serial.println("I success get date from slave");
  }
  //Serial.println(millis()-LastTimeGeting);
  // send date to Mega
  if ((Internet_flag == 1 && (millis() - LastTimeGeting) < 58000) || FirstTimeFlag)
  {
    if (Internet_flag)
    {
      Serial.println();
      Serial.println("Something happening in internet");
      writeFile(LittleFS, MyFile, GeneralString);
    }
    byte bval;
    bval = 255; // send key-start
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(bval);
    Wire.endTransmission();
    for (int i = 0; i < leng; i++)
    {
      if (InternetSensorState[i] != CurrentSensorState[i] || (FirstTimeFlag && PreviousSensorState[i] != CurrentSensorState[i]))
      {
        if (!FirstTimeFlag)
          CurrentSensorState[i] = InternetSensorState[i];
        else
          PreviousSensorState[i] = CurrentSensorState[i];
        bval = i;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write(bval);
        Wire.endTransmission();
        Serial.print(CurrentSensorState[i]);
        Serial.print(" ");
        bval = CurrentSensorState[i];
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write(bval);
        Wire.endTransmission();
      }
    }
    Wire.beginTransmission(SLAVE_ADDR);
    CurrentSensorState[leng - 1] = 254;
    Wire.write(CurrentSensorState[leng - 1]);
    Wire.endTransmission();

    // Serial.println();
    Serial.println("I sended data to mega");
    Internet_flag = 0;
    FirstTimeFlag = 0;
    delay(500);
  }
  //Serial.println("eher");
}