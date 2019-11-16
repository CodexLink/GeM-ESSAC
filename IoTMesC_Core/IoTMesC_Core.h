/*
    Name: IoT MesC Library Core Header, Two-In-One
    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/

#pragma once // A Normal Declaration of Anti-Duplicate Guard.

/*
    ! >|- Device Definitions -|<
        * |> Each device has its own functionality and with that, I want to combine the libraries and
        * functions of the two controllers for formality.
        * Please respect the device, on where you want to upload / burn the ino files. Wrong definition would occur abnormalities.

    ! NOTE
        * The name of the device was converted to Hexadecimal because of the desire of something formal. TLDR; Personal Preference
*/
#define IOT_DEVICE_CONTROLLER_AVR_DATASENS 0x4e6f64654d4355
#define IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET 0x41726475696e6f20415652

/*
    ! >|- Macro Protection Layer 1 -|<

    * |> Checks device definitions logic. It can detect both defined, or not defined.
    * It occurs errors on one of the following:
    *   |> Both Device Definitions are defined.
    *   |> Both Device Definitions are not defined.

    ! NOTE
        * Any other such illegal device definitions will not be considered.
*/

// ! Device Definition Phase 1
#if !defined(IOT_DEVICE_CONTROLLER_AVR_DATASENS) && !defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
    #error "IoTMesC Core Initialization Halted. No other such candidate device definition could be equal to current set device definition."
#elif defined(IOT_DEVICE_CONTROLLER_AVR_DATASENS) && defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
    #error "IoTMesC Core Initialization Halted. Both candidate devices are defined. Please define only one!"
#endif

// * IOT_DEVICE_CONTROLLER_AVR_DATASENS — Device Definition Declarations
#if defined(IOT_DEVICE_CONTROLLER_AVR_DATASENS)
    #ifndef IOT_DEVICE_CONTROLLER_AVR_DATASENS_GUARD
        #define IOT_DEVICE_CONTROLLER_AVR_DATASENS_GUARD
    #endif
#endif
// Macro Layer 2 Definitions for IOT_DEVICE_CONTROLLER_AVR_DATASENS

/*
    * namespace |> AVR_DEV_DECL ~= Arduino AVR Device Declarations

    *     -> It Contains All Sets of Device and Sensor Pins
    *         -> Uses Namespace and ENUM for Call Identity Uniqueness

    ! Hierarchy Device Definitions

    * |> 1. Controller
    * |> 2. Seven Segmnet Decoder
    * |> 3. SPI Interfacer
    * |> 4. I^2C Interfacer
    * |> 5. Various Sensor That Doesn't Need Any Interfacer (In short, Digital Read)
    *
    ! NOTE
        * You can use all of those declarations in a following format.
        *   |> AVR_DEV_DECL::<Device>_DEF::<PROTOCOL>_PIN_DEF
        *   |> Values cannot be replaced.
*/

/*
    * definition |> LIBRARY_INIT_GUARD ~= Library Initializer with #Define Guard

    *   -> Use for Alternation of Library Use.
    *       -> Library Hierarchy:
    *           1. LCD Sensor
    *           2. TEMP Sensor
    *           3. GAS Sensor
    *           4. Controllers and Stuff and Anything That Doesn't Need Any Fallback Library.
    *
    *       -> Possible Usage
    *           1.
    *           2.
    *           3.
    *           4.
    *
    ! NOTE
        * Not definiting fall back definitions will automatically process the arduino sketch.
        * To render with the latest sensor library that I currently have.
*/
#ifndef LIBRARY_INIT_GUARD
    #define LIBRARY_INIT_GUARD

        #ifdef LCD_FALLBACK_I2C
            #include <LiquidCrystal_I2C.h>
        #else
            // ! Include Future Library Of Those...
            // * #include
        #endif

        #ifdef TEMP_FALLBACK_SENS
            #include <DHT.h>
        #else
            // ! Include Future Library Of Those...
            // * #include
        #endif

        #ifdef GAS_FALLBACK_SENS
            #include <MQ135.h>
        #else
            // ! Include Future Library Of Those...
            // * #include
        #endif

#elif

#endif

namespace AVR_DEV_DECL
{
    // ! |> 2. Seven Segment Decoder  / Controller
    namespace SEVEN_SEG_CTRLLER_DEF : uint16_t
    {
        // ! Check for Common Pins. Please do this.
        //#ifdef COMMON_ANODE
//
        //#elif defined(COMMON_CATHODE)
//
        //#else
        //#error ""
//
        //#endif
        enum SEVEN_SEG_CONSTRAINTS : uint8_t
        {
            RESERVED_MAX_ITER = 7,
            MAX_CURR_CANDIDATE_VAL = 17

        };

        const uint8_t SevenSegment_Payload[SEVEN_SEG_CONSTRAINTS::MAX_CURR_CANDIDATE_VAL][SEVEN_SEG_CONSTRAINTS::RESERVED_MAX_ITER] = {
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

    }

    // ! |> 3. SPI Interfacer
    namespace TFT_LCD_DEF
    {
        enum SPI_PIN_DEF : uint8_t
        {
            TFT_LCD_DC = 38,
            TFT_LCD_CS = 39
        };
    }
    namespace MFRC522_DEF
    {
        enum SPI_PIN_DEF : uint8_t
        {
            RFID_RST_PIN = 8,
            RFID_SS_PIN = 53
        };
    }

    namespace RTC1302_DEF
    {
        enum DAT_PIN_DEF : uint8_t
        {
            RESPin = 10,
            SDAPin = 11,
            SCLPin = 12
        };
    }

    namespace PIR_MINI_DEF
    {
        enum DAT_PIN_DEF : uint8_t
        {
            MAIN_PIN = NULL,
            LED_TRIP_PIN = NULL
        };
    }

    namespace MQ135_DEF
    {
        enum DAT_PIN_DEF : uint8_t
        {
            MAIN_PIN = NULL
        };
    }

    namespace DIP_SWITCH_CONFIG_DEF
    {
        enum DIP_SWITCH_PIN_DEF : uint8_t
        {
            SWITCH_PIN_ONE = NULL,
            SWITCH_PIN_TWO = NULL,
            SWITCH_PIN_THREE = NULL,
            SWITCH_PIN_FOUR = NULL
        };
        enum SWITCH_DEFINED_MODE : uint8_t
        {
            MCU_RESIST_INPUT = INPUT_PULLUP,
            SWITCH_INPUT = INPUT,
            SWITCH_OUTPUT = OUTPUT
        };
    }

} // namespace AVR_PIN_DECLARATION

/*
    * namespace |> UTIL_CONST_DECL ~= Utility Constant Declarations
    * -> All Declaratives are defined here specifically used for anti-magic numbers.

*/
namespace UTIL_CONST_DECL
{
    enum CONST_ANTI_MAGIC : uint8_t
    {
        ZERO_REAL_INT = 0,
        NULL_SET_DATA = 0,
        VAL_INDEX_OFFSET = 1,
        ARR_INDEX_OFFSET = 1,
        SEG_INDEX_DP_EXEMPT_OFFSET = 2,
        CHAR_SERIAL_BUFFER_SIZE = 128

    };
    enum MILLIS_RETURN_VAL : uint8_t
    {
        CST_RET_UNKNOWN_VAL,
        CST_RET_CURR_RESULT,
        CST_RET_PREV_RESULT,
        CST_RET_INTERV_HIT
    };

    // ! Potential Backup when TFTLCD Fails. Please Elaborate more...
    enum LCD_I2C_CONSTRAINT : uint8_t
    {
        LCD_I2C_ADDR = 0x67,
        LCD_I2C_MAX_W = 20,
        LCD_I2C_MAX_H = 4,
        LCD_I2C_POS_START_X = 0,
        LCD_I2C_POS_START_Y = 0,
        LCD_I2C_POS_END_X = LCD_I2C_POS_START_X - CONST_ANTI_MAGIC::VAL_INDEX_OFFSET,
        LCD_I2C_POS_END_y = LCD_I2C_POS_START_Y - CONST_ANTI_MAGIC::VAL_INDEX_OFFSET
    };

    enum AVR_SERIAL_DECL : uint16_t
    {
        DEFAULT_PRTRCL_BAUDRATE = 0x2580, // 9600
        HOST_BAUDRATE  = 0x01c200, // 115200
        LISTEN_BAUDRATE = 0x01c200 // 115200
    };

    //#ifndef
    //#elif
    //#
}

class IoTMesC_AVR_Arduino
{

    public:
        init_DevSens();
        init_DevSPI();
    private:

    protected:
}
#endif

// * IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET — Device Definition Declarations

#if defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
#ifndef IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
#define IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
#endif
#endif