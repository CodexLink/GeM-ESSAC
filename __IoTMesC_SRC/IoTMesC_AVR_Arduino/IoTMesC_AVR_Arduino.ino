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
#include <IoTMesC_HeaderCore\IoTMesC_Core>

// * Definitions Specifically Used for AVR | Arduino
using namespace IoTMesC_AVR_DEV_DECL;
IotMesC_AVR_DRVR IoTMesC_AVR;

void setup()
{
  SerialH_Call.println(println, F("IoTMesC | Detected Arduino Arch."));
  SerialH_Call.println(println, F("IoTMesC | Initializing Required Libraries..."));
  IoTMesC_AVR.init_DevSens();
  SerialH_Call.println(println, F("IoTMesC | Done Library Initialization | Sensors."));
  IoTMesC_AVR.init_DevSPI();
  SerialH_Call.println(println, F("IoTMesC | Done Library Initialization | SPI."));
  IoTMesC_AVR.init_DevWire();
  SerialH_Call.println(println, F("IoTMesC | Done Library Initialization | Wire."));
  IoTMesC_AVR.init_DevRTC();
  SerialH_Call.println(println, F("IoTMesC | Done Library Initialization | RTC."));
  IoTMesC_AVR.init_DSD();
  SerialH_Call.println(println, F("IoTMesC | Done Library Initialization | Dual SD."));
  SerialH_Call.println(println, F("IoTMesC | Done."));
}
void loop()
{
  while (ENDLESS)
  {
    SerialH_Call.println(println, F("Awaiting for Function Calls."));
    delay(1000);
  }
}
