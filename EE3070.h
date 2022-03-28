#ifndef _EE3070_H_
#define _EE3070_H_

#include "Arduino.h"

class LED
{
    private:
        byte pin;

    public:

        LED(byte pin, bool state=LOW );
        ~LED();
        byte getPin();
        void on();
        void off();
        bool getState();
        void disattach();
};

class PUMP
{
    private:
      byte hum;

    public:

    PUMP(byte hum, bool state=LOW);//please input a threshold value as the standard humidity
    void on(); //turn on the pump
    void off(); //turn off the pump
    bool dry(); //check whether it is dry compared with input value
    int temperature(); //retutn the temperature
    int humidity(); //return the humidity
    int waterflow(); // return the waterflow
    int PH();   //return the ph
};

class LIGHT
  {
      private:
        byte threshold;
      
      public:
      LIGHT(byte threshold, bool state=LOW);//please input a threshold value as the standard brightness
      void on(); //turn on the light in automatically mode
      void userON(); // turn on the light in user control mode
      void off(); //turn off the light
      bool dark(); //check the status of the ambient light
      int brightness(); //return the brightness
  };

class ALARM
{
  private:
    byte threshold;
  
  public:
    
    ALARM(byte threshold, bool state=LOW);//please input a threshold value as the standard distance
    void on(); //turn on the buzzer
    void off(); //turn off the buzzer
    void statusLED(int d); //change the color of LED based on the distance
    bool warning(int d); //check the status 
    int distance(); //return the distance
  };

class ePaper
{
  private:
    int brightness;
    int humidity;
    int temperature;
    int PH;
    int waterflow;

  public:
    ePaper(int brightness, int humidity, int temperature, int PH, int waterflow); //please input the data you want to display
    void firstpageDisplay();
    void secondpageDisplay();
};
#endif
