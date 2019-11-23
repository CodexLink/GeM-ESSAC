/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For NodeMCU (ESP8266)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/
#pragma once


// Choices for using this library.
//#define IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET 0x41726475696e6f20415652
//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
    #if Cmd_Func == println || print
        #define SerialH_Call(Cmd_Func, Params) Serial.Cmd_Func(Params)
        #define SerialR_Call(Cmd_Func, Params) Serial1.Cmd_Func(Params)
    #else
        #define SerialH_Call(Cmd_Func, Params)
        #define SerialR_Call(Cmd_Func, Params)
    #endif
#endif


#if defined(IOT_DEVICE_CONTROLLER_AVR_DATASENS) && defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
    #error "IoTMesC Core Initialization Halted. Both candidate devices are defined. Please define only one!"
#endif

// * IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET — Device Definition Declarations
#if defined(IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET)
    #ifndef IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
       #define IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET_GUARD
    #endif

    #ifndef IOT_ESP_GUARD
        #define IOT_ESP_GUARD

            #include <ESP8266WiFi.h>
            #include <ESP8266WebServer.h>
            #include <ESP8266mDNS.h>
            #include <WiFiClient.h>
            #include "CmpntCore/SoftReset.h"

    #endif

    namespace IoTMesC_NMCU_DEV_DECL
    {
        class IoTMesC_NMCU_DRVR;

        namespace ESP_PROPERTIES
        {
            enum MyEnum
            {

            };

            enum MyEnum
            {

            };

            enum ModeFlags
            {
                DISABLED_FLAG = -2,
                ERROR_FLAG = -1,
                SUCCESS_FLAG = 0,
                IDLE_MODE = 1,
                DEBUG_MODE = 2,
                RECONATTMPT_MODE = 3,
                RETRY_MODE = 4,
                DEPENDENT_MODE = 5,
                LOW_VOLT_MODE = 6,
                EMERGENCY_MODE = 7,
                RESET_FLAG = 8

            };

        }
        namespace MyNamespace
        {
            enum MyEnum
            {

            };
            enum MyEnum
            {

            };
        }
    }

    class IoTMesC_NMCU_DEV_DECL::IoTMesC_NMCU_DRVR
    {

    public:
        // Constructor
        IoTMesC_NMCU_DRVR(void) {}
        // Destructor
        ~IoTMesC_NMCU_DRVR(void) {}

    private:
        typedef struct UserCredentials;
        typedef struct WiFiCredentials;
        static uint_fast8_t SET_HANDLE_FLAG;

        uint_fast8_t WIFI_DEFINED_COUNT;

        // * Initializers
        void begin() const;
        void init_ESPDev(ESP_PROPERTIES::ModeFlags FlagGiven);
        void init_ESPWiFi();
        // * setState FN Members


        // * WiFi Functionalities
        void WiFi_setBroadcast() noexcept;
        void WiFi_Broadcast() noexcept;
        void WiFi_Enable() noexcept;
        void WiFi_Disable() noexcept;

        // HTTP Request Handler

        // Site Handler

    };

    // Struct Declarations
    typedef struct UserCredentials
    {
        uint_fast16_t UserID;
        char *Username;
        char *Password;
    };


    typedef struct WiFiCredentials
    {
        char* WiFi_SSID;
        char* WiFi_Password;
    };

#endif // * Definition End Point |> IOT_DEVICE_CONTROLLER_ESP_SERVICE_NET

/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For NodeMCU (ESP8266)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/