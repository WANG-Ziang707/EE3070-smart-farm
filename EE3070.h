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

    PUMP(byte hum, bool state=LOW);
    void on();
    void off();
    bool dry();
    int temperature();
    int humidity();
    int waterflow();
    int PH();   
};

class LIGHT
  {
      private:
        byte threshold;
      
      public:
      LIGHT(byte threshold, bool state=LOW);
      void on();
      void userON();
      void off();
      bool dark();
      int brightness();
  };

class ALARM
{
  private:
    byte threshold;
  
  public:
    
    ALARM(byte threshold, bool state=LOW);
    void alarmon();
    void alarmoff();
    void statusLED(int d);
    bool warning(int d);
    int distance();
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
    ePaper(int brightness, int humidity, int temperature, int PH, int waterflow);
    void firstpageDisplay();
    void secondpageDisplay();
};

#endif
