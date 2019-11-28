// ! Class IoTMesC_AVR_DEV_DECL::IoTMesC_AVR_DRVR
// ! Function Definitions

#include "IoTMesC_Arduino_CoreDecl.h"

// Constructors and Destructors

IoTMesC_AVR_DRVR::IoTMesC_AVR_DRVR(uint_fast32_t BAUD_RATE)
{
    SerialH_Call(println, F("IoTMesC |> IoT, Multi-Essential Sensing Components"));
    SerialH_Call(println, F("Author |> Janrey 'CodexLink' Licas"));
    SerialH_Call(println, F("Sketch Suitable |> Arduino Variants, Recommended: MEGA"));
    SerialH_Call(println, F("Version |> Unknown"));

    _BAUD_GIVEN_RATE = BAUD_RATE;
    SerialH_Call(println, F(""));
    SerialH_Call(print, F("Baud Rate Set |> "));
    SerialH_Call(println, _BAUD_GIVEN_RATE);

    //! Initializes Displays First Before Anything. This was mandatory to check if devices are properly working.
    if (!init_DevTFT() && !init_DevDSD())
        return;
}

IoTMesC_AVR_DRVR::~IoTMesC_AVR_DRVR()
{
    SerialH_Call(println, "Device Error |> This seems to be not happening. Please consult the developer or your code, you dummy~!");
    while (INDEF_FUNC_POINT)
        ;
}

// Public Methods

void IoTMesC_AVR_DRVR::begin() const
{
    SerialH_Call(begin, _BAUD_GIVEN_RATE);

    SerialH_Call(println, F(""));
    SerialH_Call(print, F("Serial Communication, Begin @ "));
    SerialH_Call(println, _BAUD_GIVEN_RATE);

    SerialH_Call(println, F("Library Initialization |> RFID (MFRC522) Interface |> "));

    SerialH_Call(println, F("Library Initialization |> CCS811 Sensor |>"));

    SerialH_Call(println, F("Library Initialization |> SHT3X Sensor |> "));

    SerialH_Call(println, F("Library Initialization |> PIR Motion Sensor |> "));

    SerialH_Call(println, F("Library Initialization |> RTC (DS1302) Interface |> "));

    //init_DevRTC();
}

uint_fast32_t IoTMesC_AVR_DRVR::sketchTimeHit(MILLIS_RETURN_VAL ParameterCondition)
{
}

void IoTMesC_AVR_DRVR::testDSD() noexcept
{
}

void IoTMesC_AVR_DRVR::DIP_PINStats() noexcept
{
}

void IoTMesC_AVR_DRVR::initSerial_POST() noexcept
{
}

void IoTMesC_AVR_DRVR::serial_isCommsAlive() noexcept
{
}

void IoTMesC_AVR_DRVR::rtc_CheckCorrectDiff() noexcept(false)
{
}

inline void IoTMesC_AVR_DRVR::rtc_DisplayTime() noexcept
{
}

// Private Methods

// * Device and Peripherals Initializers. Initialized By Order
bool IoTMesC_AVR_DRVR::init_DevTFT()
{
}

bool IoTMesC_AVR_DRVR::init_DevDSD()
{
}

bool IoTMesC_AVR_DRVR::init_DevMFRC522()
{
}

bool IoTMesC_AVR_DRVR::init_DevCCS811()
{

}
bool IoTMesC_AVR_DRVR::init_DevSHT3X()
{

}
bool IoTMesC_AVR_DRVR::init_DevPIR()
{

}
bool IoTMesC_AVR_DRVR::init_DevDS1302()
{
}


// Device Updaters FN Members
void IoTMesC_AVR_DRVR::updateLCD() noexcept
{
}

// * Seven Segment Division
//void initDSD(/*Potential Beginning Sequence After Initialization Or Let Go*/) noexcept;
void IoTMesC_AVR_DRVR::updateDSD(/**/) noexcept
{
}

// * SPI Tranmission FN Handlers - MFRC522 and TFT_ILI9163C FN Member Declarations

// * I2C Tranmission FN Handlers – CCS811 and SHT3X FN Member Declarations
void IoTMesC_AVR_DRVR::return_CCS811_I2C(/* Possible ENUM */) noexcept
{
}

void IoTMesC_AVR_DRVR::return_SHT3X_I2C(/* Possible ENUM */) noexcept
{
}

// * Serial Communication FN Members
void IoTMesC_AVR_DRVR::serialHost_Send() noexcept
{
}

void IoTMesC_AVR_DRVR::serialHost_Receive() noexcept
{
}

void IoTMesC_AVR_DRVR::serialHost_ReEstablish() noexcept
{
}

void IoTMesC_AVR_DRVR::serialHost_Reset() noexcept // ! POTENTIAL Removal.
{
}

// * RTC FN Members
inline void IoTMesC_AVR_DRVR::rtc_PauseFN()
{
}

inline void IoTMesC_AVR_DRVR::rtc_SetWriteProtect(bool TruthValGiven) noexcept
{
}

void IoTMesC_AVR_DRVR::rtc_QueryTimeSerial() noexcept(false)
{
}

void IoTMesC_AVR_DRVR::rtc_SerialReceiveTime() noexcept
{
}