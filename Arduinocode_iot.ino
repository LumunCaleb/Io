//Include the required libraries
#include<Arduino.h>
#include<LiquidCrystal.h>
#include<DHT.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include<Wire.h>

//Declaration of variables
//RTC............................
RTC_Millis RTC;
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//End of RTC......................

//DHT.............................
const int DHTPIN = 7;   //DHT11 connected to Arduino pin7
DHT dht(7, DHT11);

float h = dht.readHumidity();
float t = dht.readTemperature();

//LCD.............................
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Rain Sensor......................
const int Rainsensor = 8;   //rain sensor is connected to Arduino pin8
int Rain = 0;

String R = "No Rain";   //R indicates rain status when raining
String N = "Raining";   //N indicates rain status when not raining

void setup ()
{
  Serial.begin(9600);

  lcd.begin(20, 4);
  dht.begin();

//The following LCD codes print the mesage that runs only once as the weather station is turned on
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("WEATHER STATION");
  lcd.setCursor(1, 1);
  lcd.print("USING INTERNET OF");
  lcd.setCursor(7, 2);
  lcd.print("THINGS");
  delay(3000);

  //DHT..............................
  digitalWrite (DHTPIN, LOW);

  //Rain sensor..................................
  pinMode(Rainsensor, INPUT);
  digitalWrite(Rainsensor, LOW);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin())
  {
    abort();
  }

  if (! rtc.isrunning())
  {

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  RTC.begin(DateTime(__DATE__, __TIME__));
}

void rtccode()
{
  lcd.clear();

  //RTC code section
  //.................................................................................
  DateTime now = rtc.now();



  lcd.clear();
  lcd.home();
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.print(" ");

  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  //End of RTC code section...............................
  void tempandhum()   //temperature and humidity function
{
  //DHTcode Section
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    lcd.setCursor(0, 1);
    lcd.print("DHT Read Error");
  }
  else
  {
    Serial.print("Temperature:  ");
    Serial.print(t);
    Serial.println("degrees");
    Serial.print("Humidity:  ");
    Serial.print(h);
    Serial.println("%");


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IP_Ad:192.168.43.109");
    lcd.setCursor(0, 1);
    lcd.print("Temperature:");
    lcd.print(t);
    lcd.print(" oC");

    lcd.setCursor(0, 2);
    lcd.print("Humidity:");
    lcd.print(h);
    lcd.print("%");


  }
}
void raincode()   //rain sensor module function
{
  Rain = digitalRead(Rainsensor);
  if (Rain == HIGH)
  {
    Serial.println("Rain Status: NO ");
    lcd.setCursor(0, 3);
    lcd.print("Rain Status:");
    lcd.print("No Rain");
  }
  else
  {
    Serial.println("Rain Status: Raining ");
    lcd.setCursor(0, 3);
    lcd.print("Rain Status:Raining");

  }
}


void loop (void)
{
//we now call the created functions in void loop
  rtccode();
  delay(1000);


  tempandhum();
  pressandaltitude();
  raincode();
  delay(2000);


  delay(2000);

}
