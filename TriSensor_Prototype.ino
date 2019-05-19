/* 
Current Name: PortESTTool-PerEMNFAD "Portable Essential Tool - Personal Environment Monitoring and Fidelity 802.11 Attacking Device"  Prototype

Old Name: DD-TriSenS "Digital Device - Tri-Sensor" Prototype

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2018
License: GPL-3.0
*/
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <SoftReset.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MQ135.h"
#define DHT22_PIN 22

#define HC595_dataPin 23
#define HC595_latchPin 24
#define HC595_clockPin 25

#define MFRC22_RST_PIN 49
#define MFRC22_SS_PIN 53

#define MQ135_GasSens A0

// Serial Identifiers and Other Definitives
#define SerialPrimary(x, y) Serial.x(y)
#define SerialSecondary(x, y) Serial1.x(y)

#define DEBUG_ENABLED 1

#ifdef DEBUG_ENABLED
#if x == println || print
#define SerialPrimary(x, y) Serial.x(y)
#define SerialSecondary(x, y) Serial1.x(y)
#endif
#else
#define SerialPrimary(x, y)
#define SerialSecondary(x, y)
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
MQ135 MQ135_Sens = MQ135(MQ135_GasSens);

// Shifter Out Initializer
ShiftOut<1> Shifter_595N;

// Char Custom Displays in Multidimensional Array
const byte SingleSegment_Values[14][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {1, 1, 1, 0, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {1, 1, 1, 1, 0, 1, 1, 0}, // 9
    {0, 0, 0, 0, 0, 0, 1, 0}, // Dash
    {1, 0, 0, 1, 1, 1, 1, 0}, // E
    {1, 0, 1, 1, 1, 1, 1, 0}, // S
    {0, 0, 0, 0, 0, 0, 0, 0}  // F
};

byte BatteryDisplay_Icon[6][8] = {
    {B01110, B01010, B10001, B10001, B10001, B10001, B10001, B11111}, // Unknown
    {B01110, B01010, B10001, B10001, B10001, B10001, B11111, B11111}, // 1 - 19
    {B01110, B01010, B10001, B10001, B10001, B11111, B11111, B11111}, // 20 - 39
    {B01110, B01010, B10001, B10001, B11111, B11111, B11111, B11111}, // 40 - 59
    {B01110, B01010, B10001, B11111, B11111, B11111, B11111, B11111}, // 60 - 79
    {B01110, B01110, B11111, B11111, B11111, B11111, B11111, B11111}  // 80 - 100
};

const byte ArrowChar_Updaters[3][8] = {
    {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100}, // Downward Arrow - 0
    {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100}, //Upward Arrow - 1
    {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000}  // No Update - default
};

// Battery Char Positional Holders
short Battery_PosX = 0, Battery_PosY = 0, BatteryIntDispX = 0, BatteryIntDispY = 0;

short LastVal_Trigger_1 = 0, LastVal_Trigger_2 = 0, DataChange_Counter[4] = {0, 0, 0, 0};

unsigned long SketchTime_Prev = 0; // Millis Value Holder

float RW_DHT22_HumidRead = 0,
      RW_DHT22_TempRead = 0,
      RW_DHT22_HtInxRead = 0;

short RW_MQ135_GasSensRead = 0;

void setup()
{
    SerialPrimary(begin, 115200);
    SerialSecondary(begin, 115200);
    SerialPrimary(println, F("Hello, Serial 0 Debugging Mode is On!"));
    SerialPrimary(println, F("[Initialization] Readying Baud Speed, LCD I2C and Shift Out"));
    Shifter_595N.begin(HC595_dataPin, HC595_clockPin, HC595_latchPin);
    DHT22_Sens.begin();
    LCD_I2C.init();
    LCD_I2C.backlight();
    LCD_I2C.setCursor(0, 0);
    LCD_I2C.print(F(" Hello and Welcome! "));
    LCD_I2C.setCursor(0, 1);
    LCD_I2C.print(F("PESTTool - PerEMNFAD"));
    LCD_I2C.setCursor(0, 2);
    LCD_I2C.print(F("    By CodexLink    "));
    LCD_I2C.setCursor(0, 3);
    LCD_I2C.print(F("Ver. Commit 05172019"));
    delay(2000);
    LCD_I2C.noBacklight();
    LCD_I2C.clear();
    delay(500);
    LCD_I2C.backlight();
    SerialPrimary(println, F("[Initialization] Ready!~"));
}
void loop()
{
    DisplayI2C_OnInstance();
}
static void DisplayI2C_OnInstance()
{
    float DHT22_TempRead = DHT22_Sens.readTemperature(),
          DHT22_HumidRead = DHT22_Sens.readHumidity(),
          DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false),
          MQ135_GasSensRead = MQ135_Sens.getPPM(); // MQ135_GasSensRead = analogRead(MQ135_GasSens);

    // Add Battery Function Display at LCD_StartPositionX and LCD_EndPositionY and Mode
    LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY);

    for (unsigned short LCD_SetScrollY = LCD_StartPositionY + 1; LCD_SetScrollY != LCD_EndPositionY; LCD_SetScrollY++)
    {
        LCD_I2C.setCursor(LCD_StartPositionX, LCD_SetScrollY);
        switch (LCD_SetScrollY)
        {
        case 1:
            // Unary Operator within Unary Operator... LOL
            (isnan(DHT22_TempRead)) ? (LCD_I2C.print(F("TE:ERROR")), ArrowChar_Indicators(LCD_StartPositionX, LCD_EndPositionY, 2)) : ((RW_DHT22_TempRead != DHT22_TempRead) ? (DataChange_Counter[0] = 1, RW_DHT22_TempRead = DHT22_TempRead) : DataChange_Counter[0] = 0);

            (RW_MQ135_GasSensRead != MQ135_GasSensRead) ? (DataChange_Counter[3] = 1, RW_MQ135_GasSensRead = MQ135_GasSensRead) : DataChange_Counter[3] = 0;
            LCD_I2C.setCursor(0, 0);
            LCD_I2C.print(F("TE:"));
            LCD_I2C.print(DHT22_TempRead, 1);
            (MQ135_GasSensRead > 999) ? LCD_I2C.print(MQ135_GasSensRead, 0) : LCD_I2C.print(MQ135_GasSensRead, 1);
        case 2:
            if (isnan(DHT22_HumidRead))
            {
                LCD_I2C.print(char(223));
                LCD_I2C.print(F("C | HU:"));
                LCD_I2C.print(DHT22_HumidRead, 1);
                LCD_I2C.print(F("%"));
            }
            else
            {
                (RW_DHT22_HumidRead != DHT22_HumidRead) ? (DataChange_Counter[1] = 1, RW_DHT22_HumidRead = DHT22_HumidRead) : DataChange_Counter[1] = 0;
            }

        case 3:
            if (isnan(DHT22_HtInxRead))
            {
                LCD_I2C.setCursor(0, 1);
                LCD_I2C.print(F("HI:"));
                LCD_I2C.print(DHT22_HtInxRead, 1);
                LCD_I2C.print(char(223));
                LCD_I2C.print(F("C | AQ:"));
            }
            else
            {
                (RW_DHT22_HtInxRead != DHT22_HtInxRead) ? (DataChange_Counter[2] = 1, RW_DHT22_HtInxRead = DHT22_HtInxRead) : DataChange_Counter[2] = 0;
            }

        default:
            DigitalSegment_Write(1);
            delay(100);
        }
    }
}

static unsigned long Current_SketchTimer(long Intervals_Millis, unsigned short Target_Result)
{
    while (1)
    {
        if ((millis() - SketchTime_Prev) >= Intervals_Millis)
        {
            SketchTime_Prev = millis();
            switch (Target_Result)
            {
            case CST_CurrentResult:
                SketchTime_Prev = 0;
                return millis();
            case CST_PreviousResult:
                return SketchTime_Prev;
            case CST_IntervalHit:
                SerialPrimary(print, millis());
                SerialPrimary(print, F(" - "));
                SerialPrimary(println, SketchTime_Prev);
                return 1;
            default:
                return CST_UnknownDefaultVal;
            }
        }
        else
        {
            SerialPrimary(print, millis());
            SerialPrimary(print, F(" - "));
            SerialPrimary(println, SketchTime_Prev);
        }
    }
}

// Display Status Indicators Hardware and I2C
static void ArrowChar_Indicators(unsigned short PosX, unsigned short PosY, unsigned short Receiver_Integer)
{
    switch (Receiver_Integer)
    {
    case 0:
        LCD_I2C.print(ArrowCharUpdaters[0][8]);
        LCD_I2C.setCursor(PosX, PosY);
    case 1:
        LCD_I2C.print(ArrowCharUpdaters[1][8]);
        LCD_I2C.setCursor(PosX, PosY);
    default:
        LCD_I2C.print(ArrowCharUpdaters[2][8]);
        LCD_I2C.setCursor(PosX, PosY);
    }
}

static void DigitalSegment_Write(byte Decimal_DisplaySwitch)
{
    static short Val = 0;
    if (Decimal_DisplaySwitch != LastVal_Trigger_1)
    {
        switch (Decimal_DisplaySwitch)
        {
        case 0:
            Shifter_595N.set(Shifter_595N.getDataWidth() - 1, 0);
            Shifter_595N.write();
            LastVal_Trigger_1 = Decimal_DisplaySwitch;
        case 1:
            Shifter_595N.set(Shifter_595N.getDataWidth() - 1, 1);
            Shifter_595N.write();
            LastVal_Trigger_1 = Decimal_DisplaySwitch;
        }
    }
    for (short i = 0; i < 4; i++)
    {
        Val += DataChange_Counter[i];
    }
    if (Val != LastVal_Trigger_2)
    {
        SerialPrimary(print, F("[SINGLE SEGMENT] Value Updated To "));
        SerialPrimary(println, Val);
        switch (Val)
        {
        case 0:
            for (short ElementCounter = 0; ElementCounter < Shifter_595N.getDataWidth() - 1; ElementCounter++)
            {
                Shifter_595N.set(ElementCounter, SingleSegment_Values[0][ElementCounter]);
                Shifter_595N.write();
            }
            LastVal_Trigger_2 = Val;
        case -1:
            Shifter_595N.setAllLow();
            Shifter_595N.write();
            LastVal_Trigger_2 = Val;
        case -2:
            Shifter_595N.setAllHigh();
            Shifter_595N.write();
            LastVal_Trigger_2 = Val;
        default:
            for (short ElementCounter = 0; ElementCounter < Shifter_595N.getDataWidth() - 1; ElementCounter++)
            {
                Shifter_595N.set(ElementCounter, SingleSegment_Values[Val][ElementCounter]);
                Shifter_595N.write();
            }
            LastVal_Trigger_2 = Val;
        }
    }
    else
    {
        SerialPrimary(print, F("[SINGLE SEGMENT] Not Updated, Value is  "));
        SerialPrimary(println, Val);
    }
}

// MRFCC RFID Functions

//static void MRFCC() {}
//static void MRFCC() {}
//static void MRFCC() {}
//static void MRFCC() {}

// Battery Indicators and Computation Functions

// This Function Below is Case Sensitive!
static void BatteryCalculation_PercentOrVolts(char *ModeDisplay)
{
    // Calculation is here

    if (strcmp(ModeDisplay, "Voltage") == 0)
    {

        SerialPrimary(println, "[Battery Calculation Mode] - Display.");
    }
    else if (strcmp(ModeDisplay, "Percent") == 0)
    {

        SerialPrimary(println, "[Battery Calculation Mode] - Display.");
    }
    else
    {
        // Might Display Percent Instead, which is Default
        SerialPrimary(println, "[Battery Calculation Mode] - Unknown Value. Set to Percent Display.");
    }
}
static short BatteryVoltagetoPercent_Write()
{
}

static void CustomCharBattery_Write(short CurrentRead_BatteryLevel)
{
    static unsigned short Battery_LastSaveLevel;
    if (Battery_LastSaveLevel != CurrentRead_BatteryLevel)
    {
        Battery_LastSaveLevel = CurrentRead_BatteryLevel;
        LCD_I2C.setCursor(Battery_PosX, Battery_PosY);
        switch (CurrentRead_BatteryLevel)
        {
        case 1 ... 19:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[1]);
            LCD_I2C.write(0);

        case 20 ... 39:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[2]);
            LCD_I2C.write(0);

        case 40 ... 59:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[3]);
            LCD_I2C.write(0);

        case 60 ... 79:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[4]);
            LCD_I2C.write(0);

        case 80 ... 100:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[5]);
            LCD_I2C.write(0);

        default:
            LCD_I2C.createChar(0, (uint8_t)BatteryDisplay_Icon[0]);
            LCD_I2C.write(0);
        }
    }
}
