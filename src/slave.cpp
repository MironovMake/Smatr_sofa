#include <Arduino.h>
// very useful site https://voltiq.ru/nodemcu-v3-connecting-to-arduino-via-i2c/
#include <Wire.h>
#define D1

//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// Stepper adjust
#include <Stepper.h>
const int stepsPerRevolution = 200;
Stepper myStepper = Stepper(stepsPerRevolution, 47, 49, 51, 53);

// TRC adjust
#include <SPI.h>
#include "RTClib.h"
RTC_DS1307 rtc;

// inputs pins:
int NumberInputs = 3;
int button[3] = {24, 25, 27};
int SensorDate[5];

// bobbers
int NumberBobber = 4;
int bobber[4] = {50, 51, 52, 53};
bool BobberState[4];

// relay
int NumberRelay = 13;
int relay[13];
bool RelayState[13];
void RelayInit()
{
  int count = 0;
  for (int i = 28; i < NumberRelay + 28; i++)
  {
    relay[count] = i;
    pinMode(relay[count], OUTPUT);
    Serial.print("pelay pin  ");
    Serial.print(i);
    Serial.print("  of relay  ");
    Serial.println(relay[count]);
    count++;
  }
  delay(5000);
}
// Функция для извлечения любых принимаемых данных от мастера на шину
void receiveEvent(int howMany)
{
  while (0 < Wire.available())
  {
    char c = Wire.read(); /* получаем байт как символ*/
                          // Serial.print(c);           /* выводим символ в серийный порт */
  }
  // Serial.println();             /* переходим на новую строку */
}
// Функция для извлечения любых отправляемых данных от мастера на шину
void requestEvent()
{
  for (int i = 0; i < 2; i++)
  {
    Wire.write(SensorDate[i]); /* отправляем по запросу строку "Hello NodeMCU" */
  }
}

void BobberRequest()
{
  Serial.println();
  for (int i = 0; i < NumberBobber; i++)
  {
    BobberState[i] = digitalRead(bobber[i]);
    Serial.print("Bobber  ");
    Serial.print(i);
    Serial.print("  is  ");
    Serial.println(BobberState[i]);
    delay(200);
  }
  Serial.println();
}

void TimeReqest()
{
  DateTime now = rtc.now();
  // Serial.print("time  ");
  SensorDate[0] = now.hour();
  SensorDate[1] = now.minute();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.println(now.minute());
}

//

void LCD_init()
{
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Mironov");
}
void setup()
{
  LCD_init();
  myStepper.setSpeed(100);
  Wire.begin(8);                /* задаем на шине i2c 8 адрес */
  Wire.onReceive(receiveEvent); /* регистрируем полученное событие */
  Wire.onRequest(requestEvent); /* регистрируем запрошенное событие */
  Serial.begin(9600);           /* открываем серийный порт для дебаггинга */
  Serial.println("arduino Mega Start");
  for (int i = 0; i < NumberBobber; i++)
  {
    pinMode(bobber[i], INPUT);
  }
  RelayInit();
}

void loop()
{
  //  TimeReqest();
  myStepper.step(2000);
  delay(2000);
  // Step on revolution in the other direction:
  myStepper.step(-2000);
  delay(2000);
  delay(100);
  BobberRequest();
  for (int i = 0; i < NumberRelay; i++)
  {
    digitalWrite(relay[i], LOW);
    Serial.print("relay numb  ");
    Serial.println(relay[i]);
    delay(100);
  }
  Serial.println("Relay is on");
  delay(2000);
  for (int i = 0; i < NumberRelay; i++)
  {
    RelayState[i] = 0;
    digitalWrite(relay[i], HIGH);
    Serial.print("relay numb  ");
    Serial.println(relay[i]);
    delay(100);
  }
  Serial.println("Relay is off");
  delay(2000);
}