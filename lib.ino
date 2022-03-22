#include "EE3070.h"

int dark = 50;
int dry = 300;
int sDistance = 500;

PUMP water(dry);
LIGHT light(dark);
ALARM alarm(sDistance);

int humidity = 0;
int brightness = 0;
int PH = 0;
int temperature = 0;
int waterflow = 0;
int distance = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = alarm.distance();
  humidity = water.humidity();
  brightness = light.brightness();
  PH = water.PH();
  temperature = water.temperature();
  waterflow = water.waterflow(); // store the data from the sensors
  
  if(water.dry()){
    water.on();
  }
  else{
    water.off();
  }
  if(light.dark()){
    light.on();
  }
  else{
    light.off();
  }
  
  if(alarm.warning(distance)){ 
    alarm.on();
    alarm.statusLED(distance);
  }
  else{
    alarm.off();
    alarm.statusLED(distance);
  }
  
  ePaper edisplay(brightness, humidity, temperature, PH,  waterflow);
}
