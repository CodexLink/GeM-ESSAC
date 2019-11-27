// ! Class IoTMesC_AVR_DEV_DECL::IoTMesC_AVR_DRVR
// ! Function Definitions

#include "IoTMesC_AVR_Core.h"

// Constructor and Destructors

/*
    ! Class Constructor |> IoTMesC_AVR_DRVR::IoTMesC_AVR_DRVR()
    * @brief |> When added, it already initializes proprietary functions that is needed.
*/
IoTMesC_AVR_DRVR::IoTMesC_AVR_DRVR(uint_fast32_t BAUD_RATE)
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
    ! Class Destructor |> IoTMesC_AVR_DRVR::~IoTMesC_AVR_DRVR()
    * @brief |> Does nothing.
*/
IoTMesC_AVR_DRVR::~IoTMesC_AVR_DRVR()
{
    // ! It is only declared to instruct that it does nothing.
    // ! We are programming an MCU which we only loop through the whole program.
}

// Public Methods
uint_fast32_t IoTMesC_AVR_DRVR::sketchTimeHit(MILLIS_RETURN_VAL ParameterCondition)
{

}

void IoTMesC_AVR_DRVR::testDSD() noexcept
{

}


// Private Methods