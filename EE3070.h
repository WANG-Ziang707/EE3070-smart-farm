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
  };


class DATA
{
  public:
  byte temperature();
  byte humidity();
  byte waterflow();
  byte PH();
  byte brightness();
};
#endif
