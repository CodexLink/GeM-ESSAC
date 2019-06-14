/*
File Name: PowerEST_CEMnCFA_NodeMCU_OOP
Programming Coding Style: Object Oriented Programming

Formal Project Name: Power Essential Tool -  Close Environment Monitoring and Common Fidelity 802.11 Attacking Device (PowerEST - CEMnCFA [802.11])
Intended and Flashable To : NodeMCU Only

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
License: GPL-3.0

NOTE: Current Stage (Procedural Program) will be the first step towards completing this project.
*/
#include <ESP8266WiFi.h>
#include <SoftReset.h>


// Serial Identifiers and Other Definitives
#define SerialComms_Host Serial
#define SerialComms_Listener Serial1

#define SerialHost_Call(x, y) Serial.x(y)
#define SerialListen_Call(x, y) Serial1.x(y)

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
    static uint8_t SET_HANDLE_FLAG;
    ESP_MainFunc(uint8_t ResistPowerFailure, uint8_t SetFlag_Mode, bool SaveOnLowState)
    {

    }

    init_SerialDefinition(const char *SerialTarget, uint16_t BaudRate);

private:
    static void PrintSerial_OnWeb(const char *Value_Identity, const char *PrintSerial_Message);
    static void set_SerialFeedback(bool LogicValue);
    static short AVR_ReadBattery(bool OutputMode = true, const char* OutputValue = "Unknown");
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
    static uint8_t ~Mode_WiFiSwitch(uint8_t SelfAwareMode, const char* DefaultStartMode);
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
