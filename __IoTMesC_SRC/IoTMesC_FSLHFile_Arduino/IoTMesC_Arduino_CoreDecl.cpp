// ! Class IoTMesC_AVR_DEV_DECL::IoTMesC_AVR_DRVR
// ! Function Definitions

#include "IoTMesC_Arduino_CoreDecl.h"

// Constructors and Destructors

IoTMesC_AVR_DRVR::IoTMesC_AVR_DRVR(uint_fast32_t BAUD_RATE)
{
    _BAUD_GIVEN_RATE = BAUD_RATE;
}

IoTMesC_AVR_DRVR::~IoTMesC_AVR_DRVR()
{
    SerialH_Call(println, "Error: This seems to be not happening. Please consult the developer or your code you dummy~!");
    while (INDEF_FUNC_POINT)
        ;
}

// Public Methods

void IoTMesC_AVR_DRVR::begin() const
{
    SerialH_Call(begin, _BAUD_GIVEN_RATE);
    SerialH_Call(println, F("IoTMesC | Detected Arduino Arch."));
    SerialH_Call(println, F("IoTMesC | Initializing Required Libraries..."));
    //init_DevSens();
    //SerialH_Call(println, F("IoTMesC | Done Library Initialization | Sensors."));
    //init_DevSPI();
    SerialH_Call(println, F("IoTMesC | Done Library Initialization | SPI."));
    //init_DevWire();
    SerialH_Call(println, F("IoTMesC | Done Library Initialization | Wire."));
    //init_DevRTC();
    SerialH_Call(println, F("IoTMesC | Done Library Initialization | RTC."));
    //init_DSD();
    SerialH_Call(println, F("IoTMesC | Done Library Initialization | Dual SD."));
    SerialH_Call(println, F("IoTMesC | Done."));
}

uint_fast32_t IoTMesC_AVR_DRVR::sketchTimeHit(MILLIS_RETURN_VAL ParameterCondition)
{
}

void IoTMesC_AVR_DRVR::testDSD() noexcept
{
}

// Private Methods