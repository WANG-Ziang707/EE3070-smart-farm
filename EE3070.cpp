#include"EE3070.h"
#include"Arduino.h"
int pump = 2;
int light = 4; //pin set up

int wet = 0;
int Brightness = 0; 
int lightness = 0; //variable set up

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


byte DATA::waterflow(){
  return analogRead(2);
}
byte DATA::PH(){
 return analogRead(3);
}
byte DATA::humidity(){
 return analogRead(4);
}
byte DATA::temperature(){
  return analogRead(5);
}
byte DATA::brightness(){
 return analogRead(6);
}
