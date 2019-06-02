
/* 
Current Name: PortESTTool-PerEMNFAD "Portable Essential Tool - Personal Environment Monitoring and Fidelity 802.11 Attacking Device" Prototype

Old Name: DD-TriSenS "Digital Device - Tri-Sensor" Prototype

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
Flashable To : Any Flashable Device, Recommended To -ESPS8266 or NodeMCU
License: GPL-3.0
*/
// From PowerESTool.ino

// Serial Identifiers and Other Definitives
#define SerialComms_Host Serial
#define SerialComms_Listener Serial1

#define SerialHost_Call(x, y) Serial.x(y)
#define SerialListen_Call(x, y) Serial1.x(y)

#include <ESP8266WiFi.h>
#include <SoftReset.h>

char *WiFi_Credientials[10][20] = {0};

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

} SetFlagHandler;

class ESP_MainFunc
{
public:
    static uint8_t DEBUG_ENABLER_FLAG;
    ESP_MainFunc(uint8_t ResistFailure, uint8_t SetFlag_Mode)
    {
        #define DEBUG_ENABLED 1
        
        #if DEBUG_ENABLED == 1
        #if x == println || print
        #define SerialHost_Call(x, y) Serial.x(y)
        #define SerialListen_Call(x, y) Serial1.x(y)
        #endif
        #else
        #define SerialHost_Call(x, y)
        #define SerialListen_Call(x, y)
        #endif
    }
    init_Serial(const char *SerialTarget, uint16_t BaudRate)
#define SerialComms_Host Serial
#define SerialComms_Listener Serial1

        private : static void PrintSerial_OnWeb(const char *Value_Identity, const char *PrintSerial_Message);
    static void set_SerialFeedback(bool LogicValue);
    static void ESP_SoftReset();

}

class ESP_WiFiHandler
{
public:
    ESP_WiFiHandler(char *SSID_Name, char *SSID_Pass, uint8_t MaxRetries, , bool EnableIndependency, char)
    {
    }
    static void Disable_WiFi();
    static void Enable_WiFi();

private:
    static uint8_t ~Connect_SSID(const char *SSID_User, const char *SSID_Pass); // Returns Success Value
    static uint8_t ~Reconnect_SSID(uint8_t Expected_RetryCount);                // Returns Success Value
    static uint8_t
};

class HTTPLocalHandler
{
public:
    HTTPLocalHandler(uint16_t IP_Port)
    {
    }

private:
};

void setup()
{
}
void loop()
{
}
