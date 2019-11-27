/*
  File Name: IoTMesC_Arduino
  Formalized Name: Iot Multi-Essential Sensing Components for Arduino

  Intended and Flashable To : Arduino MEGA Only
  @ NOTE: We will see how much SRAM will be wasted at the end of this project to ensure validity of using this in Arduino UNO

  Author Name : Janrey "CodexLink" Licas
  Created On: Unknown, Somewhere on Fall 2019
  License: GPL-3.0
*/

#define IOT_DEVICE_CONTROLLER_AVR_DATASENS 0x4e6f64654d4355
#define DEBUG_ENABLED

#include "IoTMesC_AVR_Core/IoTMesC_AVR_Core.h"

// * Definitions Specifically Used for AVR | Arduino

IotMesC_AVR_DRVR IoTMesC_AVR(115200);

void setup()
{
 ;
}

void loop()
{
  while (1)
  {
    SerialH_Call(println, F("Awaiting for Function Calls."));
    delay(1000);
  }
}
