// ! Class IoTMesC_AVR_DEV_DECL::IotMesC_AVR_DRVR
// ! Function Definitions

#define IOT_DEVICE_CONTROLLER_AVR_DATASENS 0x4e6f64654d4355
#define DEBUG_ENABLED

#include "IoTMesC_AVR_Core.h"


// Constructor and Destructors

/*
    ! Class Constructor |> IotMesC_AVR_DRVR::IotMesC_AVR_DRVR()
    * @brief |> When added, it already initializes proprietary functions that is needed.
*/
IotMesC_AVR_DRVR::IotMesC_AVR_DRVR(uint_fast32_t BAUD_RATE)
{
  SerialH_Call(begin, BAUD_RATE);
  SerialH_Call(println, F("IoTMesC | Detected Arduino Arch."));
  SerialH_Call(println, F("IoTMesC | Initializing Required Libraries..."));
  init_DevSens();
  SerialH_Call(println, F("IoTMesC | Done Library Initialization | Sensors."));
  init_DevSPI();
  SerialH_Call(println, F("IoTMesC | Done Library Initialization | SPI."));
  init_DevWire();
  SerialH_Call(println, F("IoTMesC | Done Library Initialization | Wire."));
  init_DevRTC();
  SerialH_Call(println, F("IoTMesC | Done Library Initialization | RTC."));
  init_DSD();
  SerialH_Call(println, F("IoTMesC | Done Library Initialization | Dual SD."));
  SerialH_Call(println, F("IoTMesC | Done."));
}

/*
    ! Class Destructor |> IotMesC_AVR_DRVR::~IotMesC_AVR_DRVR()
    * @brief |> Does nothing.
*/
IotMesC_AVR_DRVR::~IotMesC_AVR_DRVR()
{
    // ! It is only declared to instruct that it does nothing.
    // ! We are programming an MCU which we only loop through the whole program.
}

// Public Methods


// Private Methods