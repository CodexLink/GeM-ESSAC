/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For Arduimo (AVR)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/

#pragma once
// ! Libraries |> No Alternatives Declaration
#include "PrimaryCore/MemoryFree.h"
#include "PrimaryCore/DS1302.h"
#include "PrimaryCore/SoftReset.h"

// ! Libraries |> System Compiler Libraries Provided.
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

/*
    ! Various Libraries |> Sensors and Device
    @protocols |> I2C and SPI
    @includes |> Primary and Secondary Cores
    @usage |> #define <DEFINITION> to access libraries or alternate used libraries.
*/

#ifdef LCD_FALLBACK_I2C
    #include "AlternativeCore/LiquidCrystal_I2C.h"
#else
    #include "PrimaryCore/Adafruit_GFX.h"
    #include "PrimaryCore/TFT_ILI9163C.h"
#endif

#ifdef TEMP_FALLBACK_SENS
    #include "AlternativeCore/DHT.h"
#else
    #include "PrimaryCore/Adafruit_SHT31.h"
#endif

#ifdef GAS_FALLBACK_SENS
    #include "AlternativeCore/MQ135.h"
#else
    #include "PrimaryCore/CCS811.h"
#endif

#ifdef SERIAL_DEV_UNO
    #include <SoftwareSerial>
#endif

#ifdef SEGMENT_CUST_DECODER_ENABLE
    #include "PrimaryCore/ShiftOut.h"
#endif

#ifdef MULTIPLEXER_ENABLE
    #include "PrimaryCore/ShiftIn.h"
#endif

#ifdef MFRC_REQUIRE_EXTENDED_ENABLE
    #include "AlternativeCore/MFRC522Extended.h"
#else
    #include "PrimaryCore/MFRC522.h"
#endif


// ! Definition |> Serial Output Handlers
#define SerialH_Call(Cmd_Func, Params) Serial.Cmd_Func(Params)
#define SerialR_Call(Cmd_Func, Params) Serial1.Cmd_Func(Params)

// * Definition Macros |> Universal Usage |> Correlated to RTC Module Output as Beautifier
#define StrOtpt_RTCBeautify(ClassType, CommandGiven, StringOutput, ...) (snprintf(Formatter_Container, sizeof(Formatter_Container), StringOutput, __VA_ARGS__), ClassType.CommandGiven(Formatter_Container))

/*!
    Class Declaration |> IoTMesC_AVR_DRVR ~= IoTMesC Arduino Driver Class
    @brief
    @param


    @note
    // !
    // !

    @returns Nothing. Seriously, the class.

*/
class IoTMesC_AVR_DRVR
{
    private:
        // * ENUMs

        enum SEVEN_SEG_CONSTRAINTS : uint_fast8_t
        {
            DATA_COUNTER_ITER = 6,
            RESERVED_MAX_ITER = 7,
            MAX_CURR_CANDIDATE_VAL = 17
        };


        const uint_fast8_t SevenSegment_Payload[SEVEN_SEG_CONSTRAINTS::MAX_CURR_CANDIDATE_VAL][SEVEN_SEG_CONSTRAINTS::RESERVED_MAX_ITER] = {
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}, // 9
            {0, 0, 0, 0, 0, 0, 1}, // Dash
            {1, 1, 1, 0, 1, 1, 1}, // A
            {1, 0, 0, 1, 1, 1, 1}, // E
            {1, 0, 0, 0, 1, 1, 1}, // F
            {0, 1, 1, 0, 0, 0, 0}, // I
            {1, 0, 1, 1, 0, 1, 1}, // S
            {1, 1, 0, 0, 1, 0, 1}  // ?
        };
        // ! |> 3. Sensors and Devices | Pin Declarations
        enum TFT_LCD_PIN_DEF : uint_fast8_t
        {
            TFT_LCD_DC = 38,
            TFT_LCD_CS = 39
        };

        enum SHT3X_PIN_DEF : uint_fast16_t
        {
            SHT_ADDR = 0x44,
            SHT_ADDR_ALTER = 0x45
        };

        enum CCS811_PIN_DEF : uint_fast8_t
        {
            CCS_NWAKE = 0
        };

        enum MFRC522_PIN_DEF : uint_fast8_t
        {
            RFID_RST_PIN = 8,
            RFID_SS_PIN = 53
        };

        enum RTC1302_PIN_DEF : uint_fast8_t
        {
            RESPin = 10,
            SDAPin = 11,
            SCLPin = 12
        };

        enum PIR_MINI_PIN_DEF : uint_fast8_t
        {
            MOTION_DATA_PIN = 0,
            LED_TRIP_PIN = 0
        };

        enum MQ135_PIN_DEF : uint_fast8_t
        {
            GAS_DATA_PIN = 0
        };

        enum DIP_SWITCH_PIN_DEF : uint_fast8_t
        {
            SWITCH_PIN_ONE = 0,
            SWITCH_PIN_TWO = 0,
            SWITCH_PIN_THREE = 0,
            SWITCH_PIN_FOUR = 0
        };
        enum SWITCH_DEFINED_MODE : uint_fast8_t
        {
            MCU_RESIST_INPUT = INPUT_PULLUP,
            SWITCH_INPUT = INPUT
        };

        enum DEFINED_CONST_MAGIC : uint_fast8_t
        {
            NULL_DATA = 0,
            ZERO_REAL_INT = 0,
            NULL_SET_DATA = 0,
            VAL_INDEX_OFFSET = 1,
            ARR_INDEX_OFFSET = 1,
            INDEF_FUNC_POINT = 1,
            SEG_INDEX_DP_EXEMPT_OFFSET = 2,
            CHAR_SERIAL_BUFFER_SIZE = 128
        };
        enum MILLIS_RETURN_VAL : uint_fast8_t
        {
            CST_RET_UNKNOWN_VAL,
            CST_RET_CURR_RESULT,
            CST_RET_PREV_RESULT,
            CST_RET_INTERV_HIT
        };
        // ! Potential Backup when TFTLCD Fails. Please Elaborate more...
        enum LCD_I2C_CONSTRAINTS : int_fast8_t
        {
            LCD_I2C_ADDR = 0x67,
            LCD_I2C_MAX_W = 20,
            LCD_I2C_MAX_H = 4,
            LCD_I2C_POS_START_X = 0,
            LCD_I2C_POS_START_Y = 0,
            LCD_I2C_POS_END_X = LCD_I2C_POS_START_X - DEFINED_CONST_MAGIC::VAL_INDEX_OFFSET,
            LCD_I2C_POS_END_y = LCD_I2C_POS_START_Y - DEFINED_CONST_MAGIC::VAL_INDEX_OFFSET
        };
        enum AVR_SERIAL_DECL : uint_fast32_t
        {
            DEFAULT_PRTRCL_BAUDRATE = 0x2580, // 9600
            HOST_BAUDRATE = 0x01c200,         // 115200
            LISTEN_BAUDRATE = 0x01c200        // 115200
        };


        // * Variables
        uint_fast8_t DataCounter_Update[SEVEN_SEG_CONSTRAINTS::DATA_COUNTER_ITER] = {DEFINED_CONST_MAGIC::NULL_SET_DATA};
        static uint_fast32_t SketchTime_CurStats;
        static uint_fast8_t SerialByteCnt;
        static uint_fast16_t SRAM_FreeCnt;

        // * Variable Flag Error Holder
        bool _initErrorFlags;
        uint_fast32_t _BAUD_GIVEN_RATE;


        // * Device and Peripherals Initializers. Initialized By Order
        bool init_DevTFT();
        bool init_DevDSD();
        bool init_DevMFRC522();
        bool init_DevCCS811();
        bool init_DevSHT3X();
        bool init_DevPIR();
        bool init_DevDS1302();

        // Device Updaters FN Members
        void updateLCD() noexcept;

        // * Seven Segment Division
        //void initDSD(/*Potential Beginning Sequence After Initialization Or Let Go*/) noexcept;
        void updateDSD(/**/) noexcept;

        // * SPI Tranmission FN Handlers - MFRC522 and TFT_ILI9163C FN Member Declarations


        // * I2C Tranmission FN Handlers – CCS811 and SHT3X FN Member Declarations
        void return_CCS811_I2C(/* Possible ENUM */) noexcept;
        void return_SHT3X_I2C(/* Possible ENUM */) noexcept;


        // * Serial Communication FN Members
        void serialHost_Send() noexcept;
        void serialHost_Receive() noexcept;
        void serialHost_ReEstablish() noexcept;
        void serialHost_Reset() noexcept; // ! POTENTIAL Removal.


        // * RTC FN Members
        inline void rtc_PauseFN();
        inline void rtc_SetWriteProtect(bool TruthValGiven) noexcept;
        void rtc_QueryTimeSerial() noexcept(false);
        void rtc_SerialReceiveTime() noexcept;


    public:
    /*
        ! Class Constructor (Explicit Declaration) |> IoTMesC_AVR_DRVR()
        * @brief |> When added, it puts given baud to another variable to initialize with.
    */
        IoTMesC_AVR_DRVR(uint_fast32_t BAUD_RATE);
        ~IoTMesC_AVR_DRVR();

        void begin() const;
        uint_fast32_t sketchTimeHit(MILLIS_RETURN_VAL ParameterCondition);
        void testDSD(/*ENUM or Choices Here.*/) noexcept;
        void DIP_PINStats() noexcept;

        void initSerial_POST() noexcept;
        void serial_isCommsAlive() noexcept;

        void rtc_CheckCorrectDiff() noexcept(false);
        inline void rtc_DisplayTime() noexcept;
};

/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For Arduimo (AVR)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/