/*
  File Name: IoTMesC_Arduino
  Formalized Name: Iot Multi-Essential Sensing Components for Arduino

  Intended and Flashable To : Arduino MEGA Only
  @ NOTE: We will see how much SRAM will be wasted at the end of this project to ensure validity of using this in Arduino UNO

  Author Name : Janrey "CodexLink" Licas
  Created On: Unknown, Somewhere on Fall 2019
  License: GPL-3.0
*/

#include "IoTMesC_Arduino_CoreDecl.h"
#define ON_STATE(LED_PIN) digitalWrite(LED_PIN, HIGH)
#define OFF_STATE(LED_PIN) digitalWrite(LED_PIN, LOW)
#define PWM_STATE(LED_PIN, BRIGHTNESS) analogWrite(LED_PIN, BRIGHTNESS)

IoTMesC_AVR_DRVR IoTMesC_AVR(0x01c200);

//inline void PowerLED_UP(uint8_t pin, uint16_t delayseq = 1, uint16_t delayframe = 1);
inline void PowerLED_Wait();

void setup()
{
  IoTMesC_AVR.begin();
}

void loop()
{
  while (1)
  {
    SerialH_Call(println, F("Awaiting for Function Calls."));
    PowerLED_Wait();
    //PowerLED_UP(10);
    //PowerLED_UP(11);
    //PowerLED_UP(12);
    //delay(1000);
  }
}

inline void PowerLED_Wait()
{
  for (int i = 10; i < 13; i++)
  {

    for (int a = 0; a <= 254; a = a + 2)
    {
      PWM_STATE(i, a);
      delay(1);
    }
  }
  delay(5);
  for (int i = 10; i < 13; i++)
  {
    for (int a = 254; a >= 0; a = a - 2)
    {
      PWM_STATE(i, a);
      delay(1);
    }
  }
  delay(100);
}
inline void PowerLED_UP(uint8_t pin, uint16_t delayseq = 1, uint16_t delayframe = 1)
{
  for (int a = 0; a <= 255; a++)
  {
    PWM_STATE(pin, a);
    delay(delayframe);
  }
  delay(delayseq);
  for (int a = 255; a >= 0; a--)
  {
    PWM_STATE(pin, a);
    delay(delayframe);
  }
}