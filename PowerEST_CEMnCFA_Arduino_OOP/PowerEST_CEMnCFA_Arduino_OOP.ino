/* 
File Name: PowerEST_CEMnCFA_Arduino_OOP
Programming Coding Style: Object Oriented Programming

Formal Project Name: Power Essential Tool -  Close Environment Monitoring and Common Fidelity 802.11 Attacking Device (PowerEST - CEMnCFA [802.11])
Intended and Flashable To : Arduino MEGA Only, We will see how much SRAM will be wasted at the end of this project to ensure validity of using this in Arduino UNO

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
License: GPL-3.0

NOTE: OOP Style of this project will be starting on / held on Maintaining Stage of the project.
*/

#include <stdio.h>

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ShiftOut.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MQ135.h>
#include <MemoryFree.h>
#include <DS1302.h>

#define RTCResetPin 10
#define RTCDataPin 11
#define RTCClockPin 12

#define DHT22_PIN 22

#define HC595_dataPin 23
#define HC595_latchPin 24
#define HC595_clockPin 25

#define SwitchMode_One 26
#define SwitchMode_Two 27

// This was due to common annode or cathode.

#define Switch_ContainMode(x) ((digitalRead(x) == INVERSE_HIGH) ? 1 : 0)
#define Switch_ModeUpdateCheck(x, y) (y != x) ? (LCD_I2C.clear(), y = x) : (0)
#define ModeTransitionStart(x) (LCD_I2C.setCursor(0, 0), LCD_I2C.print(x))

#define INVERSE_LOW HIGH
#define INVERSE_HIGH LOW

#define MFRC22_RST_PIN 49
#define MFRC22_SS_PIN 53

#define MQ135_GasSens A0

// Serial Identifiers and Other Definitives
#define SerialComms_Host Serial
#define SerialComms_Listener Serial1

#define SerialHost_Call(x, y) Serial.x(y)
#define SerialListen_Call(x, y) Serial1.x(y)

// Serial / LCD I2C Concatenation Display Formmater, Requires a Global Variable of Container
#define RTC_MAX_BUFFER_SIZE 50
#define String_BeautifyRTC(ClassType, CommandGiven, StringOutput, ...) (snprintf(Formatter_Container, sizeof(Formatter_Container), StringOutput, __VA_ARGS__), ClassType.CommandGiven(Formatter_Container))

#define DEBUG_ENABLED 0

#if DEBUG_ENABLED == 1
#if x == println || print
#define SerialHost_Call(x, y) Serial.x(y)
#define SerialListen_Call(x, y) Serial1.x(y)
#endif
#else
#define SerialHost_Call(x, y)
#define SerialListen_Call(x, y)
#endif

// Software Serial Last Save State of Pins Used
//#define AVR_RX_Equiv 40
//#define AVR_TX_Equiv 3
//SoftwareSerial Serial1(AVR_RX_Equiv, AVR_TX_Equiv);

// NodeMCU Errors (To Be Continued)
//#define Node_Stats_Enable 0xCD1200
//#define Node_Stats_Disable 0xCD0000

// Customized Millis Timer Definition Return Values
#define CST_CurrentResult 1
#define CST_PreviousResult 2
#define CST_IntervalHit 3
#define CST_UnknownDefaultVal 0420

// Default LCD Postional Definitions
#define LCD_StartPositionX 0
#define LCD_StartPositionY 0
#define LCD_EndPositionX 19
#define LCD_EndPositionY 3

// Sensor Initializers
DHT DHT22_Sens(DHT22_PIN, DHT22);
LiquidCrystal_I2C LCD_I2C(0x27, 20, 4);
DS1302 RTCModule(RTCResetPin, RTCDataPin, RTCClockPin);
MQ135 MQ135_Sens = MQ135(MQ135_GasSens);

// Shifter Out Initializer
ShiftOut<1> Shifter_595N;

// Display Formatter Container. MAX_SIZE'd for Ease.
char Formatter_Container[RTC_MAX_BUFFER_SIZE] = {0};
// Char Custom Displays in Multidimensional Array
const uint8_t SingleSegment_Values[17][7] = {
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

const uint8_t BatteryDisplay_Icon[6][8] = {
    {B01110, B01010, B10001, B10001, B10001, B10001, B10001, B11111}, // Unknown
    {B01110, B01010, B10001, B10001, B10001, B10001, B11111, B11111}, // 1 - 19
    {B01110, B01010, B10001, B10001, B10001, B11111, B11111, B11111}, // 20 - 39
    {B01110, B01010, B10001, B10001, B11111, B11111, B11111, B11111}, // 40 - 59
    {B01110, B01010, B10001, B11111, B11111, B11111, B11111, B11111}, // 60 - 79
    {B01110, B01110, B11111, B11111, B11111, B11111, B11111, B11111}  // 80 - 100
};

const uint8_t ArrowChar_UpdateDisp[3][8] = {
    {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100}, // Downward Arrow - 0
    {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100}, //Upward Arrow - 1
    {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000}  // No Update - default
};

// Battery Char Positional Holders
uint16_t Battery_PosX = 0, Battery_PosY = 0, BatteryIntDispX = 0, BatteryIntDispY = 0;

// 7 Digit Segment and Sensor Computation Comparison Data Variable Handlers
uint8_t ArrowChar_LiveContainer[6][8] = {0};
uint8_t DataCounter_Update[6] = {0};

// Customized Millis Previous On Hold Value
uint32_t SketchTime_Prev = 0;

void setup()
{

}
void loop()
{
    DisplayI2C_OnInstance(); // I don;t wanna pollute this part so create another function instead.
}

// LOOP FUNCTION Start
DisplayI2C_OnInstance()
{
    
}