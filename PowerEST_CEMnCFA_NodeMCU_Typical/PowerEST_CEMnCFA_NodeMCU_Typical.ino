/* 
File Name: PowerEST_CEMnCFA_Arduino_Procedural
Programming Coding Style: Procedural Programming

Formal Project Name: Power Essential Tool -  Close Environment Monitoring and Common Fidelity 802.11 Attacking Device (PowerEST - CEMnCFA [802.11])
Intended and Flashable To : NodeMCU Only

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
License: GPL-3.0

NOTE: Current Stage (Procedural Program) will be the first step towards completing this project.
*/
#include <ESP8266WiFi.h>
//#include <SoftReset.h>

// From Arduino_PoerESTool.ino

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

void setup()
{
    SerialComms_Host.begin(115200);
    SerialComms_Listener.begin(11500);
    pinMode(2, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
}
void loop()
{
    digitalWrite(2, HIGH); // HAVE THIS DEFINITION
    digitalWrite(LED_BUILTIN, HIGH);
    SerialComms_Host.write(SerialComms_Host.read());
    SerialComms_Listener.println("Hello World this one is has total of 62 bytes.");
    SerialComms_Host.println("Hello World this one is has total of 62 bytes.");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(2, LOW);
}

