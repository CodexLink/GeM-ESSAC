/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For NodeMCU (ESP8266)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/
#pragma once

// ! Libraries |> Main Dependency
#include "CmpntCore/ESP8266WiFi/ESP8266WiFi.h"
#include "CmpntCore/ESP8266mDNS/ESP8266mDNS.h"
#include "CmpntCore/ESP8266WebServer/ESP8266WebServer.h"
//#include <WiFiClient.h>

// ! Libraries |> System Compiler Libraries Provided.
#include <SPI.h>

#ifdef DEBUG_ENABLED
#if Cmd_Func == println || print
#define SerialH_Call(Cmd_Func, Params) Serial.Cmd_Func(Params)
#define SerialR_Call(Cmd_Func, Params) Serial1.Cmd_Func(Params)
#else
#define SerialH_Call(Cmd_Func, Params)
#define SerialR_Call(Cmd_Func, Params)
#endif
#endif

class IoTMesC_NMCU_DRVR
{
private:
    typedef struct UserCredentials;
    typedef struct WiFiCredentials;
    enum ESP_ModeFlags;
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
    public:
    // Constructor
    IoTMesC_NMCU_DRVR(void) {}
    // Destructor
    ~IoTMesC_NMCU_DRVR(void) {}

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
    char *WiFi_SSID;
    char *WiFi_Password;
};

enum ESP_ModeFlags
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
/*
    Name: IoT Multi-Essential Sensing Components | For Personal Benefits
    Type: Library Core Header | Declarations Only For NodeMCU (ESP8266)

    Created by: Janrey "CodexLink" Licas
    Created on: 11/14/2019 - 21:23
*/