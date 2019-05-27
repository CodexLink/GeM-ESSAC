/* 
Current Name: PortESTTool-PerEMNFAD "Portable Essential Tool - Personal Environment Monitoring and Fidelity 802.11 Attacking Device" Prototype

Old Name: DD-TriSenS "Digital Device - Tri-Sensor" Prototype

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
Flashable To : Any Flashable Device, Recommended To - Arduino
License: GPL-3.0
*/
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ShiftOut.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MQ135.h"
#include <MemoryFree.h>

#define DHT22_PIN 22

#define HC595_dataPin 23
#define HC595_latchPin 24
#define HC595_clockPin 25

#define MFRC22_RST_PIN 49
#define MFRC22_SS_PIN 53

#define MQ135_GasSens A0

// Serial Identifiers and Other Definitives
#define SerialComms_Host Serial
#define SerialComms_Listen Serial1

#define SerialHost_Call(x, y) Serial.x(y)
#define SerialListen_Call(x, y) Serial1.x(y)

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
const uint8_t SingleSegment_Values[14][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // Removed 8th Index, 0
    {0, 1, 1, 0, 0, 0, 0}, // Removed 8th Index, 1
    {1, 1, 0, 1, 1, 0, 1}, // Removed 8th Index, 2
    {1, 1, 1, 1, 0, 0, 1}, // Removed 8th Index, 3
    {0, 1, 1, 0, 0, 1, 1}, // Removed 8th Index, 4
    {1, 0, 1, 1, 0, 1, 1}, // Removed 8th Index, 5
    {1, 0, 1, 1, 1, 1, 1}, // Removed 8th Index, 6
    {1, 1, 1, 0, 0, 0, 0}, // Removed 8th Index, 7
    {1, 1, 1, 1, 1, 1, 1}, // Removed 8th Index, 8
    {1, 1, 1, 1, 0, 1, 1}, // Removed 8th Index, 9
    {0, 0, 0, 0, 0, 0, 1}, // Removed 8th Index, Dash
    {1, 0, 0, 1, 1, 1, 1}, // Removed 8th Index, E
    {1, 0, 1, 1, 1, 1, 1}, // Removed 8th Index, S
    {0, 0, 0, 0, 0, 0, 0}  // Removed 8th Index, F
};

const uint8_t BatteryDisplay_Icon[6][8] = {
    {B01110, B01010, B10001, B10001, B10001, B10001, B10001, B11111}, // Unknown
    {B01110, B01010, B10001, B10001, B10001, B10001, B11111, B11111}, // 1 - 19
    {B01110, B01010, B10001, B10001, B10001, B11111, B11111, B11111}, // 20 - 39
    {B01110, B01010, B10001, B10001, B11111, B11111, B11111, B11111}, // 40 - 59
    {B01110, B01010, B10001, B11111, B11111, B11111, B11111, B11111}, // 60 - 79
    {B01110, B01110, B11111, B11111, B11111, B11111, B11111, B11111}  // 80 - 100
};

const uint8_t ArrowChar_UpdateDisps[3][8] = {
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
    SerialHost_Call(begin, 115200);
    SerialListen_Call(begin, 115200);
    SerialHost_Call(println, F("Hello, Serial 0 Debugging Mode is On!"));
    SerialHost_Call(println, F("[Initialization] Readying Baud Speed, LCD I2C and Shift Out"));
    Shifter_595N.begin(HC595_dataPin, HC595_clockPin, HC595_latchPin);
    DHT22_Sens.begin();
    LCD_I2C.init();
    LCD_I2C.backlight();
    LCD_I2C.setCursor(0, 0);
    LCD_I2C.print(F(" Hello and Welcome! "));
    LCD_I2C.setCursor(0, 1);
    LCD_I2C.print(F("PortESTL - PerEMNFAD"));
    LCD_I2C.setCursor(0, 2);
    LCD_I2C.print(F("    By CodexLink    "));
    LCD_I2C.setCursor(0, 3);
    LCD_I2C.print(F("Ver. Commit 05262019"));
    delay(2000);
    LCD_I2C.noBacklight();
    LCD_I2C.clear();
    delay(500);
    LCD_I2C.backlight();
    SerialHost_Call(println, F("[Initialization] Ready!~"));
}
void loop()
{
    DisplayI2C_OnInstance(); // I don;t wanna pollute this part so create another function instead.
}

// LOOP FUNCTION Start
static void DisplayI2C_OnInstance()
{
    // Sensor Value Initializers
    static float RW_DHT22_HumidRead = 0,
                 RW_DHT22_TempRead = 0,
                 RW_DHT22_HtInxRead = 0;
    static uint16_t RW_MQ135_GasSensRead = 0;
    // Insert Command Here for Reading Functions....

    static uint16_t SRAM_RecentFreeMem;
    static uint8_t Serial_RecentByteCount;

    float DHT22_TempRead = DHT22_Sens.readTemperature(),
          DHT22_HumidRead = DHT22_Sens.readHumidity(),
          DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false);
    uint16_t MQ135_GasSensRead = MQ135_Sens.getPPM(); // MQ135_GasSensRead = analogRead(MQ135_GasSens);
    // Function on DisplayI2C_OnInstance: Dynamically Arranges Next Print Character Based on Length Returned
    SerialHost_Call(println, F("")); // For Debugging Purposes

    for (size_t LCDScrollY_Index = LCD_StartPositionY, LCD_SetScrollX = 0; LCDScrollY_Index <= LCD_EndPositionY; LCDScrollY_Index++)
    {
        switch (LCDScrollY_Index)
        {
        case 0: // Ready the battery here in this case. I don't want to read it outside, don't ask why.
            static uint8_t BatteryCurrentRead = Battery_CapCalc();
            CustomCharBattery_Write(BatteryCurrentRead, LCD_StartPositionX, LCDScrollY_Index);
            BatteryDisp_Format(BatteryCurrentRead, "Capacity");
            CommsHeader_Status();
            break;

        case 1:
            if (isnan(DHT22_TempRead))
            {
                DataSens_DispUpdater(0, 0, LCD_StartPositionX, LCDScrollY_Index, "DHT22-TEMP");
                LCD_I2C.print(F("TE"));
                LCD_I2C.write(126);
                LCD_I2C.print(F("ERROR"));

                DataSens_DispUpdater(MQ135_GasSensRead, RW_MQ135_GasSensRead, LCD_StartPositionX + 11, LCDScrollY_Index, "MQ135");

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_MQ135_GasSensRead = MQ135_GasSensRead) : (0);
                (MQ135_GasSensRead > 999) ? (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), LCD_I2C.print(F("999+"))) : (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), LCD_I2C.print(MQ135_GasSensRead, DEC));
                break;
            }
            else
            {
                DataSens_DispUpdater(DHT22_TempRead, RW_DHT22_TempRead, LCD_StartPositionX, LCDScrollY_Index, "DHT22-TEMP");

                (DataCounter_Update[LCDScrollY_Index - 1]) ? (RW_DHT22_TempRead = DHT22_TempRead, LCD_I2C.print(F("TE")), LCD_I2C.write(126), LCD_I2C.print(DHT22_TempRead, 1), LCD_I2C.print(F("C"))) : (0);

                DataSens_DispUpdater(MQ135_GasSensRead, RW_MQ135_GasSensRead, LCD_StartPositionX + 11, LCDScrollY_Index, "MQ135");

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_MQ135_GasSensRead = MQ135_GasSensRead) : (0);

                (MQ135_GasSensRead >= 999) ? (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), LCD_I2C.print(F("999+"))) : (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), LCD_I2C.print(MQ135_GasSensRead, DEC));
                break;
            }
            // I can't read Sensor Disconnections, the only way is to put resistor and read something about it. ALl I know is that I should be able to tell if that <something> is low...

        case 2:
            if (isnan(DHT22_HumidRead))
            {
                DataSens_DispUpdater(0, 0, LCD_StartPositionX, LCDScrollY_Index, "DHT22-HUMID");
                LCD_I2C.print(F("HU"));
                LCD_I2C.write(126);
                LCD_I2C.print(F("ERROR"));

                DataSens_DispUpdater(freeMemory(), SRAM_RecentFreeMem, LCD_StartPositionX + 11, LCDScrollY_Index, "SRAM-USAGE");

                (DataCounter_Update[LCDScrollY_Index]) ? (SRAM_RecentFreeMem = freeMemory(), LCD_I2C.print(F("FM")), LCD_I2C.write(126), LCD_I2C.print(freeMemory()), LCD_I2C.print(F("B"))) : (0);

                break;
            }
            else
            {
                DataSens_DispUpdater(DHT22_HumidRead, RW_DHT22_HumidRead, LCD_StartPositionX, LCDScrollY_Index, "DHT22-HUMID");

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_DHT22_HumidRead = DHT22_HumidRead, LCD_I2C.print(F("HU")), LCD_I2C.write(126), LCD_I2C.print(DHT22_HumidRead, 1), LCD_I2C.print(F("%"))) : (0);

                DataSens_DispUpdater(freeMemory(), SRAM_RecentFreeMem, LCD_StartPositionX + 11, LCDScrollY_Index, "SRAM-USAGE");

                (DataCounter_Update[LCDScrollY_Index]) ? (SRAM_RecentFreeMem = freeMemory(), LCD_I2C.print(F("FM")), LCD_I2C.write(126), LCD_I2C.print(freeMemory()), LCD_I2C.print(F("B"))) : (0);

                break;
            }

        case 3:
            if (isnan(DHT22_HtInxRead))
            {
                DataSens_DispUpdater(0, 0, LCD_StartPositionX, LCDScrollY_Index, "DHT22-HT_INX");
                LCD_I2C.print(F("HI"));
                LCD_I2C.write(126);
                LCD_I2C.print(F("ERROR"));

                DataSens_DispUpdater(SerialReceiver_ByteCount(), Serial_RecentByteCount, LCD_StartPositionX + 11, LCDScrollY_Index, "Serial_ByteCount");

                (DataCounter_Update[LCDScrollY_Index]) ? (Serial_RecentByteCount = SerialReceiver_ByteCount(), LCD_I2C.print(F("SB")), LCD_I2C.write(126), LCD_I2C.print(SerialReceiver_ByteCount()), LCD_I2C.print(F("B"))) : (0);
                break;
            }
            else
            {
                DataSens_DispUpdater(DHT22_HtInxRead, RW_DHT22_HtInxRead, LCD_StartPositionX, LCDScrollY_Index, "DHT22-HT_INX");

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_DHT22_HtInxRead = DHT22_HtInxRead, LCD_I2C.print(F("HI")), LCD_I2C.write(126), LCD_I2C.print(DHT22_HtInxRead, 1), LCD_I2C.print(F("C"))) : (0);

                DataSens_DispUpdater(SerialReceiver_ByteCount(), Serial_RecentByteCount, LCD_StartPositionX + 11, LCDScrollY_Index, "Serial_ByteCount");

                (DataCounter_Update[LCDScrollY_Index]) ? (Serial_RecentByteCount = SerialReceiver_ByteCount(), LCD_I2C.print(F("SB")), LCD_I2C.write(126), LCD_I2C.print(SerialReceiver_ByteCount()), LCD_I2C.print(F("B"))) : (0);
                break;
            }
        }
    }
    SegmentDisp_Update();
}

// LOOP FUNCTION END Section

/*Arduino Base Function, Customized Millis Timer for Clean and Elegant Calling of the Typical If-Else Statement */
static uint32_t Current_SketchTimer(uint32_t Intervals_Millis, uint16_t Target_Result)
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
                break;
            case CST_PreviousResult:
                return SketchTime_Prev;
                break;
            case CST_IntervalHit:
                return 1;
                break;
            default:
                return CST_UnknownDefaultVal;
                break;
            }
        }
        else
        {
            return 0;
        }
    }
}

/* 
Mainstream Functions Section - Set of Functions that utilize the whole processes by displaying,
computing, comparing, storing, outputting, transferring of the data from Sensors or from Arduino Data itself.

DataSens_DispUpdater(float Base Reading, float Recent Reading, uint8_t Index from Loop Fnction, const char* Sensor, WhereItComesFrom Identity) - Extended Dependent Variable for Digital Segment Updater. Extended for Functionality of On-Display LCD Arrow Char Difference Value from Last Save and Current Read of Sensors.

*/
static short DataSens_DispUpdater(float BasePinSensRead, float SaveState_RecentRead, uint8_t LCD_PosX, uint8_t Sens_LCD_CaseIndex, const char *SensorIdentifier)
{
    uint8_t Compare_ReturnVal;
    SerialHost_Call(print, F("[SENS READ, "));
    SerialHost_Call(print, SensorIdentifier);
    SerialHost_Call(print, F("] > ( BaseReadVal > "));
    SerialHost_Call(print, BasePinSensRead);
    SerialHost_Call(print, F(" |<| LastReadVal > "));
    SerialHost_Call(print, SaveState_RecentRead);

    if (BasePinSensRead < SaveState_RecentRead)
    {
        SerialHost_Call(println, F(" ) Returns 0, Less Than"));
        Compare_ReturnVal, DataCounter_Update[Sens_LCD_CaseIndex] = 0;
    }
    else if (BasePinSensRead > SaveState_RecentRead)
    {
        SerialHost_Call(println, F(" ) Returns 1, Greater Than"));
        Compare_ReturnVal, DataCounter_Update[Sens_LCD_CaseIndex] = 1;
    }
    else
    {
        SerialHost_Call(println, F(" ) Returns 2, Equal"));
        Compare_ReturnVal, DataCounter_Update[Sens_LCD_CaseIndex] = 2;
    }
    for (size_t ArrIterator = 0; ArrIterator <= 8; ArrIterator++)
    {
        ArrowChar_LiveContainer[Sens_LCD_CaseIndex][ArrIterator] = ArrowChar_UpdateDisps[Compare_ReturnVal][ArrIterator];
    }

    LCD_I2C.createChar(1, ArrowChar_LiveContainer[Sens_LCD_CaseIndex]);
    LCD_I2C.setCursor(LCD_PosX, Sens_LCD_CaseIndex);
    LCD_I2C.write(1);
}

//static void ArrowChar_UpdateDisp(uint8_t PosX, uint8_t PosY, uint8_t Value_CaseIndex)
//{
//    LCD_I2C.createChar(1, ArrowChar_LiveContainer[Value_CaseIndex]);
//    LCD_I2C.setCursor(PosX, PosY);
//    LCD_I2C.write(1);
//}

// Battery Character Write and Computation Section

static void CustomCharBattery_Write(short CurrentRead_BatteryLevel, uint16_t LCD_StartPosX, uint16_t LCD_StartPosY)
{
    switch (CurrentRead_BatteryLevel)
    {
    case 1 ... 19:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[1]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;

    case 20 ... 39:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[2]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;

    case 40 ... 59:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[3]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;

    case 60 ... 79:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[4]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;

    case 80 ... 100:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[5]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;

    default:
        LCD_I2C.createChar(0, BatteryDisplay_Icon[0]);
        LCD_I2C.setCursor(LCD_StartPosX, LCD_StartPosY);
        LCD_I2C.write(0);
        break;
    }
}

static uint8_t Battery_CapCalc()
{

    //if (Current_SketchTimer(500, CST_IntervalHit))
    //{
    for (size_t ReturnValue = 100; ReturnValue != 0; ReturnValue--)
    {
        return ReturnValue;
    }
    for (size_t ReturnValue = 0; ReturnValue != 100; ReturnValue++)
    {
        return ReturnValue;
    }
    //}
}
// Battery Display Formatter - This Function Below is Case Sensitive! Also this funciton, will run once. Implementation of OTA Changes will be implemented soon when the whole project is at final stage.

static const char *BatteryDisp_Format(uint16_t BatteryLoad, const char *ModeDisplay)
{
    const char DisplayFormat[2] = {'V', '%'};
    static float BatteryBaseValue = 0;
    static const char *ModeDisplay_SaveState;
    // Calculation is here
    if (strcmp(ModeDisplay, ModeDisplay_SaveState) != 0)
    {
        if (strcmp(ModeDisplay, "Voltage") == 0)
        {
            SerialHost_Call(println, F("[Battery Calculation Mode] - Set to Voltage Display."));
            LCD_I2C.print(BatteryLoad);
            LCD_I2C.print(DisplayFormat[0]);
            LCD_I2C.print(F(" "));
        }
        else if (strcmp(ModeDisplay, "Capacity") == 0)
        {
            SerialHost_Call(println, F("[Battery Calculation Mode] - Set to Battery Percentage Display."));
            LCD_I2C.print(BatteryLoad);
            LCD_I2C.print(DisplayFormat[1]);
            LCD_I2C.print(F(" "));
        }
        else
        {
            // Might Display Percent Instead, which is Default
            SerialHost_Call(println, F("[Battery Calculation Mode] - Unknown Value. Set to Percent Display."));
        }
    }
}

/*Seven Digit - Digital Segment Display Function */

static void SegmentDisp_Update()
{
    static uint16_t LastSave_TotalSumOnArr = 0;
    static uint8_t LastSave_DecSwitch, Current_DecSwitch = 1;

    uint16_t Current_TotalSumOnArr = 0; // Reports Total Sum of DataCounter_Update

    // Just in case, I wasn't kind of woke enough from knowing what this switch-case do. This one access 8th element of the array from Decimal Point.
    // Reads Data from DataCounter_Update
    SerialHost_Call(print, F("[Digit Segment Array] > |"));
    for (size_t ArrayAccess = 0; ArrayAccess < 4; ArrayAccess++)
    {
        Current_TotalSumOnArr += DataCounter_Update[ArrayAccess];
        SerialHost_Call(print, DataCounter_Update[ArrayAccess]);
    }
    SerialHost_Call(println, F("|"));

    if (Current_TotalSumOnArr != LastSave_TotalSumOnArr)
    {
        SerialHost_Call(print, F("[SINGLE SEGMENT] Total Sum Value Updated > "));
        SerialHost_Call(println, Current_TotalSumOnArr);
        for (size_t DigitalSegment_WriteIterator = 0; DigitalSegment_WriteIterator <= Shifter_595N.getDataWidth() - 2; DigitalSegment_WriteIterator++)
        {
            Shifter_595N.set(DigitalSegment_WriteIterator, SingleSegment_Values[Current_TotalSumOnArr][DigitalSegment_WriteIterator]);
        }
        Shifter_595N.write();
        LastSave_TotalSumOnArr = Current_TotalSumOnArr;
    }
    else
    {
        SerialHost_Call(print, F("[SINGLE SEGMENT] Total Sum Value Not Updated > "));
        SerialHost_Call(println, Current_TotalSumOnArr);
    }

    if (Current_SketchTimer(1000, CST_IntervalHit))
    {

        if (Current_DecSwitch != LastSave_DecSwitch)
        {
            Shifter_595N.set(Shifter_595N.getDataWidth() - 1, Current_DecSwitch);
            Shifter_595N.write();
            switch (Current_DecSwitch)
            {
            case 0:
                LastSave_DecSwitch = Current_DecSwitch;
                Current_DecSwitch = 1;
                break;
            case 1:
                LastSave_DecSwitch = Current_DecSwitch;
                Current_DecSwitch = 0;
                break;
            }
        }
    }
}

// Device Communication Section - Set of Functions that needs report of Serial.

// Device Communications Status Display - Max of 14 Words
static void CommsHeader_Status()
{
    LCD_I2C.print(F("Debugging Mode"));
}

static signed int SerialReceiver_ByteCount()
{
    return SerialComms_Host.available();
}

static void SerialHost_SendComms(char *AT_CommandGiven)
{
    ;
}

static uint8_t SerialReceiver_DataCompile()
{
    ;
}