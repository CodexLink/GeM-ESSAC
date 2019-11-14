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

// Macro Layer 2 Definitions for IOT_DEVICE_CONTROLLER_AVR_DATASENS

//#define
#endif

/*
    * |> AVR_DEV_DECL ~= Arduino AVR Device Declarations

    *     -> It Contains All Sets of Device and Sensor Pins
    *         -> Uses Namespace and ENUM for Call Identity Uniqueness

    ! Hierarchy Device Definitions

    * |> 1. Controller
    * |> 2. Seven Segmnet Decoder
    * |> 3. SPI Interfacer
    * |> 4. I^2C Interfacer
    * |> 5. Various Sensor That Doesn't Need Any Interfacer (In short, Digital Read)
*/

namespace AVR_DEV_DECL
{
    // ! |> 3. SPI Interfacer
    namespace TFT_DEF
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


} // namespace AVR_PIN_DECLARATION

namespace DEFINITION
{}
#endif

// * IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET — Device Definition Declarations

#if defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
#ifndef IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
#define IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
#endif
#endif