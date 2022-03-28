#include"EE3070.h"
#include"Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <GxEPD2_BW.h> 
#include <Fonts/FreeMonoBold9pt7b.h> 
#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h"
#include <math.h>

 


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define ENABLE_GxEPD2_GFX 0
#define MAX_DISPLAY_BUFFER_SIZE 800 
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=10*/ 10, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEH0154D67 200x200

const int pump = 2;
const int buzzer = 3;
const int LED_blue_light=4;
const int LED_green_light = 5;
const int LED_red_light = 6;
const int distanceecho = 11;
const int distancetrig = 12;
const int light = 36;//pin set up

float len1 = 0;
float len2 = 0;
float len3 = 0;
int Distance = 0;
int tDistance = 0;
int wet = 0;
int Brightness = 0;
float duration = 0; 
int lightness = 0; 
float real_temperature = 0;
float real_lightness = 0;
float real_humidity = 0;//variable set up


//--------------------------------------------------------------------------------------------------

LED::LED(byte p,bool state):pin(p)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,state);
}
LED::~LED(){
  disattach();
}
void LED::on()
{
  digitalWrite(pin,HIGH);
}

void LED::off(){
  digitalWrite(pin,LOW);
}
bool LED::getState(){
  return digitalRead(pin);
}

void LED::disattach(){
  digitalWrite(pin,LOW);
  pinMode(pin,INPUT);
}

PUMP::PUMP(byte hum, bool state)
{
  pinMode(pump,OUTPUT);
  digitalWrite(pump,state);
}
void PUMP::on()
{
  digitalWrite(pump,HIGH);
}
void PUMP::off(){
  digitalWrite(pump,LOW);
}
bool PUMP::dry(){
  wet = analogRead(4);
  if(wet > hum){
    return true;
  }
  else {
    return false;
  }
}
int PUMP::temperature(){
  real_temperature = analogRead(5);
  real_temperature = (5 * real_temperature * 100) / 1024;
  return real_temperature;
}
int PUMP::humidity(){
  return analogRead(4);
}
int PUMP::PH(){
  return analogRead(3);
}
int PUMP::waterflow(){
  return analogRead(2);
}


LIGHT::LIGHT(byte threshold, bool state)
{
  pinMode(light, OUTPUT);
  digitalWrite(light,state);
}
void LIGHT::on(){
  lightness = analogRead(6);
  Brightness = (threshold - lightness) * 5;
  analogWrite(light,Brightness);
}
void LIGHT::off(){
  digitalWrite(light, LOW);
}
void LIGHT::userON(){
  digitalWrite(light,HIGH);
}
bool LIGHT::dark(){
  lightness = analogRead(6);
  if(lightness<threshold){
    return true;
  }
  else{
    return false;
  }
}
int LIGHT::brightness(){
  real_lightness = analogRead(6);
  real_lightness = pow(350*(16.5/real_lightness - 3.3),-1.43);
  return real_lightness;
}

ALARM::ALARM(byte threshold, bool state = LOW){
  tDistance = threshold;
  pinMode(distanceecho,INPUT);
  pinMode(distancetrig,OUTPUT);
  pinMode(LED_blue_light, OUTPUT);
  pinMode(LED_green_light, OUTPUT);
  pinMode(LED_red_light, OUTPUT);
  pinMode(buzzer,OUTPUT);
}
int ALARM::distance(){
  digitalWrite(distancetrig, LOW);
  delayMicroseconds(2);
  digitalWrite(distancetrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(distancetrig, LOW);
  duration = pulseIn(distanceecho, HIGH);
  Distance = duration * 0.034 / 2;
  return Distance;
}
void ALARM::on(){
  tone(buzzer,500);
}
void ALARM::off(){
  noTone(buzzer);
}
bool ALARM::warning(int d){
  if(d<tDistance){
    return true;
  }
  else{
    return false;
  }
}
void ALARM::statusLED(int d){
  if(d<tDistance){
    digitalWrite(LED_red_light,HIGH);
    digitalWrite(LED_green_light,LOW);
  }
  else{
    digitalWrite(LED_red_light,LOW);
    digitalWrite(LED_green_light,HIGH);
  }
}

ePaper::ePaper(int brightness, int humidity, int temperature, int PH, int waterflow){
  len1=((float)(1023-(float)humidity)/1023)*150;
  len2 = (float)((float)brightness/1023)*150;
  len3 = (float)((float)temperature/1023)*150;    
}

void ePaper::firstpageDisplay(){
 display.init();;
 display.firstPage();
 do {
 display.setTextColor(GxEPD_BLACK);
 display.fillScreen(GxEPD_WHITE);
 display.setFont(&FreeMonoBold9pt7b);
 display.setTextSize(1);

display.setCursor(20, 10);
display.println("Light: ");
display.setCursor(130, 10);
display.println(brightness);

display.setCursor(20, 55);
display.println("Humidity: ");
display.setCursor(130, 55);
display.println(humidity);

display.setCursor(20, 100);
display.println("Temp: ");
display.setCursor(130, 100);
display.println(temperature);

display.setCursor(20, 145);
display.println("PH: ");
display.setCursor(130, 145);
display.println(PH);

display.setCursor(20, 190);
display.println("w_flow: ");
display.setCursor(130, 190);
display.println(waterflow);

 } while (display.nextPage());
display.powerOff();
}

void ePaper::secondpageDisplay(){
   display.init();;
   display.firstPage();
        
 do {
  
 display.setTextColor(GxEPD_BLACK);
 display.fillScreen(GxEPD_WHITE);
 display.setFont(&FreeMonoBold9pt7b);
 display.setTextSize(1);
      
display.setCursor(0, 35);
display.fillRect(50,35,len1,20,GxEPD_BLACK);
display.println("H: ");


display.setCursor(0, 90);
display.fillRect(50,90,len2,20,GxEPD_BLACK);
display.println("L: ");


display.setCursor(0, 145);
display.fillRect(50,145,len3,20,GxEPD_BLACK);
display.println("T: ");



 } while (display.nextPage());
display.powerOff();
}
