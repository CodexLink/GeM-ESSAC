/* 
File Name: PowerEST_CEMnCFA_Arduino_Procedural
Programming Coding Style: Procedural Programming

Formal Project Name: Power Essential Tool -  Close Environment Monitoring and Common Fidelity 802.11 Attacking Device (PowerEST - CEMnCFA [802.11])
Intended and Flashable To : Arduino MEGA Only, We will see how much SRAM will be wasted at the end of this project to ensure validity of using this in Arduino UNO

Author Name : Janrey "CodexLink" Licas
Created On: Unknown, Somewhere on Fall 2019
License: GPL-3.0

NOTE: Current Stage (Procedural Program) will be the first step towards completing this project.
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

#define PIRMini_MotionSens 28
#define PIRMini_LEDTrip 29

#define Switch_ContainMode(x) ((digitalRead(x) == INVERSE_HIGH) ? 1 : 0) // This was due to common annode or cathode.
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

#define PIRSensor_MotionCheck(x,y) (digitalRead(x) == HIGH) ? digitalWrite(y, HIGH) : digitalWrite(y, LOW)

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

// Magic Numbers to Value Literal Significants
#define ZERO_INT 0 // For Variable Initialization
#define NULL_SET 0 // For Array Set Intialization
#define ARR_INDEX_OFFSET 1 // For - ARR_INDEX_OFFSET Statements
#define SEG_INDEX_DP_EXEMPT 2 // Decimal Point @ Segment Display is Not Included...
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

// Device Baud Rates
#define HOST_BAUDRATE 115200
#define LISTEN_BAUDRATE 115200

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
    {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100}, //Upward Arrow - ARR_INDEX_OFFSET
    {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000}  // No Update - default
};

// Battery Char Positional Holders
uint16_t Battery_PosX = ZERO_INT, Battery_PosY = ZERO_INT, BatteryIntDispX = ZERO_INT, BatteryIntDispY = ZERO_INT;

// 7 Digit Segment and Sensor Computation Comparison Data Variable Handlers
uint8_t ArrowChar_LiveContainer[6][8] = {NULL_SET};
uint8_t DataCounter_Update[6] = {NULL_SET};

// Serial Communication Parameters and Variables

#define CHARACTER_BUFFER_SIZE 128

char SerialCommunication_Container[CHARACTER_BUFFER_SIZE] = {NULL_SET};

// Customized Millis Previous On Hold Value
uint32_t SketchTime_Prev = ZERO_INT;

void setup()
{
    SerialHost_Call(begin, HOST_BAUDRATE);
    SerialListen_Call(begin, LISTEN_BAUDRATE);
    SerialHost_Call(println, F("Hello, Serial 0 Debugging Mode is On!"));
    SerialHost_Call(println, F("[Initialization] Readying Baud Speed, LCD I2C, Shift Out Object and RTC"));
    Shifter_595N.begin(HC595_dataPin, HC595_clockPin, HC595_latchPin);
    DHT22_Sens.begin();
    LCD_I2C.init();
    pinMode(PIRMini_MotionSens, INPUT);
    pinMode(PIRMini_LEDTrip, OUTPUT);
    digitalWrite(PIRMini_MotionSens, LOW);
    digitalWrite(PIRMini_LEDTrip, LOW);
    RTC_PauseFunction(false);
    RTC_WriteProtection(false);
    RTC_PrototypeInit();
    SerialHost_Call(println, F("[Initialization] Setting Two Pins with PULLUP for the Switches of LCD Modes..."));
    pinMode(SwitchMode_One, INPUT_PULLUP);
    pinMode(SwitchMode_Two, INPUT_PULLUP);
    SerialHost_Call(println, F("[Initialization] Setting Two Pins with PULLUP Done..."));
    LCD_I2C.backlight();
    LCD_I2C.setCursor(0, 0);
    LCD_I2C.print(F(" Hello and Welcome! "));
    LCD_I2C.setCursor(0, 1);
    LCD_I2C.print(F(" PowerEST - CEMnCFA "));
    LCD_I2C.setCursor(0, 2);
    LCD_I2C.print(F("    By CodexLink    "));
    LCD_I2C.setCursor(0, 3);
    LCD_I2C.print(F("Ver. Commit 06182019"));
    delay(2000);
    LCD_I2C.noBacklight();
    LCD_I2C.clear();
    delay(500);
    LCD_I2C.backlight();
    SerialHost_Call(println, F("[Initialization] Executing Device POST Before Actual Program Execution..."));
    if (DisplayI2C_ShowPOST())
    {
        SerialHost_Call(println, F("[Initialization] POST Checks Result is Passed. Ready!~"));
    }
    else
    {
        SerialHost_Call(println, F("[Initialization] Some POST Checks are not passed! Please check them!"));
    }
    delay(2500);
    LCD_I2C.clear();
}
void loop()
{
    DisplayI2C_OnInstance(); // I don't wanna pollute this part so create another function instead.
}

// POST Function Display
static uint16_t DisplayI2C_ShowPOST()
{
    uint8_t Checkpoint_Values[3] = {NULL_SET};
    uint8_t ReturnResultValue;
    for (size_t LoopIteration = ZERO_INT; LoopIteration <= LCD_EndPositionY; LCD_I2C.setCursor(LCD_StartPositionX, ++LoopIteration))
    {
        switch (LoopIteration)
        {
        case 0:
            LCD_I2C.write(126);
            LCD_I2C.print(F("Device POST"));
            break;

        case 1:
            LCD_I2C.print(F("Segment Disp "));
            Checkpoint_Values[LoopIteration - ARR_INDEX_OFFSET] = 1;
            LCD_I2C.write(126);
            LCD_I2C.print(F("PASSED"));
            break;
        case 2:
            LCD_I2C.print(F("Serial Comms "));
            Checkpoint_Values[LoopIteration - ARR_INDEX_OFFSET] = 1;
            LCD_I2C.write(126);
            LCD_I2C.print(F("PASSED"));
            break;
        case 3:
            LCD_I2C.print(F("Voltage Ind. "));
            Checkpoint_Values[LoopIteration - ARR_INDEX_OFFSET] = 1;
            LCD_I2C.write(126);
            LCD_I2C.print(F("PASSED"));
            break;
        }
    }
    if (!Checkpoint_Values[0] || !Checkpoint_Values[1] || !Checkpoint_Values[2])
    {
        return ReturnResultValue = ZERO_INT;
    }
    else
    {
        return ReturnResultValue = 1;
    }
}
// LOOP FUNCTION Start
static void DisplayI2C_OnInstance()
{
    // Sensor Value Initializers
    static float RW_DHT22_HumidRead,
        RW_DHT22_TempRead,
        RW_DHT22_HtInxRead;
    static uint8_t RW_SwitchLCD_ScreenMode_One, RW_SwitchLCD_ScreenMode_Two;
    static uint8_t Serial_RecentByteCount = 1;
    static uint16_t RW_MQ135_GasSensRead;
    static uint16_t SRAM_RecentFreeMem;
    // Insert Command Here for Reading Functions....
    float DHT22_TempRead = DHT22_Sens.readTemperature(),
          DHT22_HumidRead = DHT22_Sens.readHumidity(),
          DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false);
    uint8_t SwitchLCD_ScreenMode[2] = {Switch_ContainMode(SwitchMode_One), Switch_ContainMode(SwitchMode_Two)};
    uint16_t MQ135_GasSensRead = MQ135_Sens.getCorrectedPPM(DHT22_TempRead, DHT22_HumidRead);

    // Function on DisplayI2C_OnInstance: Dynamically Arranges Next Print Character Based on Length Returned

    PIRSensor_MotionCheck(PIRMini_MotionSens, PIRMini_LEDTrip);

    Switch_ModeUpdateCheck(SwitchLCD_ScreenMode[0], RW_SwitchLCD_ScreenMode_One);
    Switch_ModeUpdateCheck(SwitchLCD_ScreenMode[1], RW_SwitchLCD_ScreenMode_Two);

    SerialHost_Call(println, F("")); // For Debugging Purposes

    if (SwitchLCD_ScreenMode[0] && !SwitchLCD_ScreenMode[1])
    {
        for (size_t LCDScrollY_Index = LCD_StartPositionY, LCD_SetScrollX = ZERO_INT; LCDScrollY_Index <= LCD_EndPositionY; LCDScrollY_Index++)
        {
            switch (LCDScrollY_Index)
            {
            case 0: // Ready the battery here in this case. I don't want to read it outside, don't ask why.
                static uint8_t BatteryCurrentRead = Battery_CapCalc();
                CustomCharBattery_Write(BatteryCurrentRead, LCD_StartPositionX, LCDScrollY_Index);

                BatteryDisp_Format(BatteryCurrentRead, "Capacity");
                RTC_Display_GetChipCurrentTime();
                //CommsHeader_Status();
                break;

            case 1:

                DataSens_DispUpdater(DHT22_TempRead, RW_DHT22_TempRead, LCD_StartPositionX, LCDScrollY_Index, '-', 1, "DHT22-TEMP");

                (isnan(DHT22_TempRead)) ? (LCD_I2C.print(F("TE")), LCD_I2C.write(126), LCD_I2C.print(F("Error "))) : (LCD_I2C.print(F("TE")), LCD_I2C.write(126), LCD_I2C.print(DHT22_TempRead, 1), LCD_I2C.print(F("C")));

                (DataCounter_Update[LCDScrollY_Index - ARR_INDEX_OFFSET]) ? (RW_DHT22_TempRead = DHT22_TempRead) : (0);

                DataSens_DispUpdater(MQ135_GasSensRead, RW_MQ135_GasSensRead, LCD_StartPositionX + 11, LCDScrollY_Index, '+', 0, "MQ135");

                (!MQ135_GasSensRead) ? (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), LCD_I2C.print(F("Error"))) : (LCD_I2C.print(F("AQ")), LCD_I2C.write(126), /* LCD_I2C.print(MQ135_GasSensRead, DEC)*/ LCDWrite_AwareSpaceInt(MQ135_GasSensRead, 5));

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_MQ135_GasSensRead = MQ135_GasSensRead) : (0);

                break;
                // I can't read Sensor Disconnections, the only way is to put resistor and read something about it. ALl I know is that I should be able to tell if that <something> is low...

            case 2:
                DataSens_DispUpdater(DHT22_HumidRead, RW_DHT22_HumidRead, LCD_StartPositionX, LCDScrollY_Index, '+', 0, "DHT22-HUMID");

                (isnan(DHT22_HumidRead)) ? (LCD_I2C.print(F("HU")), LCD_I2C.write(126), LCD_I2C.print(F("Error "))) : (LCD_I2C.print(F("HU")), LCD_I2C.write(126), LCD_I2C.print(DHT22_HumidRead, 1), LCD_I2C.print(F("%")));

                (DataCounter_Update[LCDScrollY_Index]) ? (RW_DHT22_HumidRead = DHT22_HumidRead) : (0);
                DataSens_DispUpdater(freeMemory(), SRAM_RecentFreeMem, LCD_StartPositionX + 11, LCDScrollY_Index, '+', 1, "SRAM-USAGE");
                (DataCounter_Update[LCDScrollY_Index + 1]) ? (SRAM_RecentFreeMem = freeMemory(), LCD_I2C.print(F("FM")), LCD_I2C.write(126), LCD_I2C.print(freeMemory()), LCD_I2C.print(F("B"))) : (LCD_I2C.print(F("FM")), LCD_I2C.write(126), LCD_I2C.print(SRAM_RecentFreeMem, DEC), LCD_I2C.print(F("B")));
                break;

            case 3:
                DataSens_DispUpdater(DHT22_HtInxRead, RW_DHT22_HtInxRead, LCD_StartPositionX, LCDScrollY_Index, '+', 1, "DHT22-HT_INX");

                (isnan(DHT22_HtInxRead)) ? (LCD_I2C.print(F("HI")), LCD_I2C.write(126), LCD_I2C.print(F("Error "))) : (LCD_I2C.print(F("TE")), LCD_I2C.write(126), LCD_I2C.print(DHT22_HtInxRead, 1), LCD_I2C.print(F("C")));
                (DataCounter_Update[LCDScrollY_Index + 1]) ? (RW_DHT22_HtInxRead = DHT22_HtInxRead) : (0);

                DataSens_DispUpdater(SerialComms_Host.available(), Serial_RecentByteCount, LCD_StartPositionX + 11, LCDScrollY_Index, '+', 2, "Serial_ByteCount");

                (DataCounter_Update[LCDScrollY_Index + 2]) ? (Serial_RecentByteCount = SerialComms_Host.available(), LCD_I2C.print(F("SB")), LCD_I2C.write(126), LCD_I2C.print(SerialComms_Host.available()), LCD_I2C.print(F("B"))) : (LCD_I2C.print(F("SB")), LCD_I2C.write(126), LCD_I2C.print(Serial_RecentByteCount, DEC), LCD_I2C.print(F("B")));
                break;
            }
        }
        SegmentDisp_Update(false, '0');
    }
    else if (SwitchLCD_ScreenMode[0] && SwitchLCD_ScreenMode[1])
    {
        SegmentDisp_Update(true, 'F');
        static char WDeauMode_StringContainers[4][20] = {
            {"Attacking"},
            {"HW_EcLi284255H."},
            {"Node32"},
            {"WAIT"},
        };

        static uint8_t WDeauMode_IntegerContainers[3] = {15, 254, 164};

        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY);
        LCD_I2C.print(F("WDeau "));
        LCD_I2C.write(126);
        LCD_I2C.print(F(" "));
        LCD_I2C.print(WDeauMode_StringContainers[0]);

        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 1);
        LCD_I2C.print(F("SSID"));
        LCD_I2C.write(126);
        LCD_I2C.print(WDeauMode_StringContainers[1]);

        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 2);

        LCD_I2C.print(F("Deauth"));
        LCD_I2C.write(126);
        LCD_I2C.print(F("254.13"));
        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 3);
        LCD_I2C.print(F("MAC"));
        LCD_I2C.write(126);
        LCD_I2C.print(F("88D7F69C39F0"));
    }
    else if (!SwitchLCD_ScreenMode[0] && SwitchLCD_ScreenMode[1])
    {
        static char NodeMCUST_StringContainers[3][20] = {
            {"Dependent"},
            {"HW_EcLi284255H."},
            {"Node32"}};
        //                                              3rd,  4th, Current, Max
        static uint8_t NodeMCUST_IntegerContainers[4] = {254, 164, 10, 10};

        SegmentDisp_Update(true, '-');
        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY);
        LCD_I2C.print(F("NodeST"));
        LCD_I2C.write(126);
        LCD_I2C.print(NodeMCUST_StringContainers[0]);

        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 1);
        LCD_I2C.print(F("SSID"));
        LCD_I2C.write(126);
        LCD_I2C.print(NodeMCUST_StringContainers[1]);
        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 2);

        LCD_I2C.print(F("-CC"));
        LCD_I2C.write(126);
        LCD_I2C.print(NodeMCUST_IntegerContainers[2]);
        LCD_I2C.print(F("/"));
        LCD_I2C.print(NodeMCUST_IntegerContainers[3]);

        LCD_I2C.print(F("  "));
        LCD_I2C.print(F("IP"));
        LCD_I2C.write(126);
        LCD_I2C.print(NodeMCUST_IntegerContainers[1]);
        LCD_I2C.print(F("."));
        LCD_I2C.print(NodeMCUST_IntegerContainers[2]);

        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 3);
        LCD_I2C.print(F("-SB"));
        LCD_I2C.write(126);
        LCD_I2C.print(F("0B"));

        LCD_I2C.print(F("     PW"));
        LCD_I2C.write(126);
        LCD_I2C.print(NodeMCUST_StringContainers[2]);
    }
    else if (!SwitchLCD_ScreenMode[0] && !SwitchLCD_ScreenMode[1])
    {
        bool isuserMode_Technical = true;
        //char UserModeString[12] = {"osu! Client"}; // Create Another one and Define a mode.
        char UserModeString[12] = {"Technical"}; // Create Another one and Define a mode.
        LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY);
        LCD_I2C.print(F("WinMode"));
        LCD_I2C.write(126);
        LCD_I2C.print(UserModeString);
        if (isuserMode_Technical)
        {
            SegmentDisp_Update(true, '-');
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 1);
            LCD_I2C.print(F("Mp"));
            LCD_I2C.write(126);
            LCD_I2C.print(F("ClariS,Hitorigoto"));
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 2);
            LCD_I2C.print(F("121.21pp "));
            LCD_I2C.write(126);
            LCD_I2C.print(F(" 860/860pp"));
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 3);
            LCD_I2C.print(F("GaugeSpd"));
            LCD_I2C.write(126);
            LCD_I2C.print(F("-----------"));
        }
        else
        {
            SegmentDisp_Update(true, '-');
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 1);
            LCD_I2C.print(F("-BT"));
            LCD_I2C.write(126);
            LCD_I2C.print(F("5432KB"));
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 2);
            LCD_I2C.print(F("LLS"));
            LCD_I2C.write(126);
            LCD_I2C.print(F("LCKD | 12:00AM"));
            LCD_I2C.setCursor(LCD_StartPositionX, LCD_StartPositionY + 3);
            //LCD_I2C.print(F("Stats"));
            //LCD_I2C.write(126);
            //LCD_I2C.print(F("Good"));
            LCD_I2C.print(F("AC"));
            LCD_I2C.write(126);
            LCD_I2C.print(F("12/20"));
        }
    }
}

// LOOP FUNCTION END Section

/*Arduino Base Function, Customized Millis Timer for Clean and Elegant Calling of the Typical If-Else Statement */
static uint32_t SketchTime_IntervalHit(uint32_t Intervals_Millis)
{
    if (millis() - SketchTime_Prev >= Intervals_Millis)
    {
        SketchTime_Prev = millis();
        return 1;
    }
    else
    {
        return 0;
    }
}

/*LCD Custom Function for Clearing Unwanted Characters has 2 functions for each well known data types. One for Integer and String. The idea about this was basically from the title already. The logic behind this one is tricky thats because
when we gonna overwrite a character, we have to read it's source length - spaces. And the result one will be the # for each space that needs to be generate to overwrite unwanted characters. I seperate those two due to the fact that we have difference of reading the arguments passed. 


*/

static int LCDWrite_AwareSpaceInt(uint16_t PassedValue, uint8_t SpaceSize)
{
    // https://stackoverflow.com/questions/3068397/finding-the-length-of-an-integer-in-c
    uint32_t intlen = floor(log10(abs(PassedValue))) + 1; // I understand this, except I forgot how calculation of log10 does. Closer calculation concept I know is byte to base 10 (decimal)
    LCD_I2C.print(PassedValue);
    for (size_t SpaceManipulate = ZERO_INT; SpaceManipulate < SpaceSize - intlen; SpaceManipulate++)
    {
        LCD_I2C.print(F(" "));
    }
}

static void LCDWrite_AwareSpaceStr(const char *PassedString, uint8_t SpaceSize)
{
    LCD_I2C.print(PassedString);
    for (uint8_t SpaceManipulate = ZERO_INT; SpaceManipulate < SpaceSize - strlen(PassedString); SpaceManipulate++)
    {
        LCD_I2C.print(F(" "));
    }
}
/* 
Mainstream Functions Section - Set of Functions that utilize the whole processes by displaying,
computing, comparing, storing, outputting, transferring of the data from Sensors or from Arduino Data itself.

DataSens_DispUpdater(float Base Reading, float Recent Reading, uint8_t Index from Loop Fnction, const char* Sensor, WhereItComesFrom Identity) - Extended Dependent Variable for Digital Segment Updater. Extended for Functionality of On-Display LCD Arrow Char Difference Value from Last Save and Current Read of Sensors.

*/
static short DataSens_DispUpdater(float BasePinSensRead, float SaveState_RecentRead, uint8_t LCD_PosX, uint8_t Sens_LCD_CaseIndex, char Arith_Operator, uint8_t ElementOffset, const char *Value_Indentifier)
{
    uint8_t Compare_ArrowReturnIndex, Compare_DataCounterUpdate, LCD_writeCharIndex;

    SerialHost_Call(print, F("[SENS READ, "));
    SerialHost_Call(print, Value_Indentifier);
    SerialHost_Call(print, F("] - ( BaseReadVal > "));
    SerialHost_Call(print, BasePinSensRead);
    SerialHost_Call(print, F(", LastReadVal > "));
    SerialHost_Call(print, SaveState_RecentRead);

    if (BasePinSensRead < SaveState_RecentRead)
    {
        SerialHost_Call(println, F(" ) Returns Less Than, (0 @ ArrowChar_Container, 1 on DataCounter_Update)"));
        Compare_ArrowReturnIndex = ZERO_INT;
        Compare_DataCounterUpdate = 1;
        LCD_writeCharIndex = 1;
    }
    else if (BasePinSensRead > SaveState_RecentRead)
    {
        SerialHost_Call(println, F(" ) Returns Greater Than, (1 @ ArrowChar_Container, 1 on DataCounter_Update)"));
        Compare_ArrowReturnIndex = 1;
        Compare_DataCounterUpdate = 1;
        LCD_writeCharIndex = 2;
    }
    else
    {
        SerialHost_Call(println, F(" ) Returns Equal, (2 @ ArrowChar_Container, 0 on DataCounter_Update)"));
        Compare_ArrowReturnIndex = 2;
        Compare_DataCounterUpdate = ZERO_INT;
        LCD_writeCharIndex = 3;
    }

    SerialHost_Call(print, "Value Identifier -> ");
    SerialHost_Call(print, Value_Indentifier);
    SerialHost_Call(print, " has been placed on Pos (");
    SerialHost_Call(print, LCD_PosX);
    SerialHost_Call(print, ", ");
    SerialHost_Call(println, Sens_LCD_CaseIndex);
    SerialHost_Call(print, ")");

    LCD_I2C.createChar(LCD_writeCharIndex, ArrowChar_UpdateDisp[Compare_ArrowReturnIndex]);
    LCD_I2C.setCursor(LCD_PosX, Sens_LCD_CaseIndex);
    LCD_I2C.write(LCD_writeCharIndex);

    (Arith_Operator == '+') ? (DataCounter_Update[Sens_LCD_CaseIndex + ElementOffset] = Compare_DataCounterUpdate) : (DataCounter_Update[Sens_LCD_CaseIndex - ElementOffset] = Compare_DataCounterUpdate);
}

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
    static uint8_t Static_BatterLevelStress = 95;

    return Static_BatterLevelStress;
}
// Battery Display Formatter - This Function Below is Case Sensitive! Also this funciton, will run once. Implementation of OTA Changes will be implemented soon when the whole project is at final stage.

static char *BatteryDisp_Format(uint16_t BatteryLoad, const char *ModeDisplay)
{
    const char DisplayFormat[2] = {'V', '%'};
    static float BatteryBaseValue = ZERO_INT;

    static uint8_t LastSave_FormatIndex;

    static char ModeDisplay_SaveState[10] = "NULL";
    // Calculation is here

    if (strcmp(ModeDisplay, ModeDisplay_SaveState) != ZERO_INT)
    {
        if (strcmp(ModeDisplay, "Voltage") == ZERO_INT)
        {
            SerialHost_Call(println, F("[Battery Calculation Mode] - Set to Voltage Display."));
            LastSave_FormatIndex = ZERO_INT;
            LCD_I2C.print(BatteryLoad);
            LCD_I2C.print(DisplayFormat[LastSave_FormatIndex]);
            LCD_I2C.print(F(" "));
            strncpy(ModeDisplay_SaveState, "Voltage", sizeof("Voltage"));
        }
        else if (strcmp(ModeDisplay, "Capacity") == ZERO_INT)
        {
            SerialHost_Call(println, F("[Battery Calculation Mode] - Set to Battery Percentage Display."));
            LastSave_FormatIndex = 1;
            LCD_I2C.print(BatteryLoad);
            LCD_I2C.print(DisplayFormat[LastSave_FormatIndex]);
            LCD_I2C.print(F(" "));

            strncpy(ModeDisplay_SaveState, "Capacity", sizeof("Capacity"));
        }
        else
        {
            // Might Display Percent Instead, which is Default
            SerialHost_Call(println, F("[Battery Calculation Mode] - Unknown Value. Please check the parameters set on sketch."));
        }
    }
    else
    {
        LCD_I2C.print(BatteryLoad);
        LCD_I2C.print(DisplayFormat[LastSave_FormatIndex]);
        LCD_I2C.print(F(" "));
    }
}

/*Seven Digit - Digital Segment Display Function */

// This POST Designed Function will be used for iterating all characters defined in the array bytes display.
static void SegmentDisp_InitPOST()
{
    ;
}

static void SegmentDisp_Update(bool isLoadedCustomChar, char CustomCharacterParams)
{
    static uint16_t LastSave_TotalSumOnArr = ZERO_INT;
    static uint8_t LastSave_DecSwitch, Current_DecSwitch = 1;

    uint16_t Current_TotalSumOnArr = ZERO_INT; // Reports Total Sum of DataCounter_Update

    if (!isLoadedCustomChar)
    {
        // Just in case, I wasn't kind of woke enough from knowing what this switch-case do. This one access 8th element of the array from Decimal Point.
        // Reads Data from DataCounter_Update
        SerialHost_Call(print, F("[Digit Segment Array] > |"));
        for (size_t ArrayAccess = ZERO_INT; ArrayAccess < 6; ArrayAccess++)
        {
            Current_TotalSumOnArr += DataCounter_Update[ArrayAccess];
            SerialHost_Call(print, DataCounter_Update[ArrayAccess]);
        }
        SerialHost_Call(println, F("|"));

        if (Current_TotalSumOnArr != LastSave_TotalSumOnArr)
        {
            SerialHost_Call(print, F("[SINGLE SEGMENT] Total Sum Value Updated > "));
            SerialHost_Call(println, Current_TotalSumOnArr);
            for (size_t DigitalSegment_WriteIterator = ZERO_INT; DigitalSegment_WriteIterator <= Shifter_595N.getDataWidth() - SEG_INDEX_DP_EXEMPT; DigitalSegment_WriteIterator++)
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
    }
    else
    {
        switch (CustomCharacterParams)
        {
        case '-':
            Current_TotalSumOnArr = 10;
            break;
        case 'A':
            Current_TotalSumOnArr = 11;
            break;
        case 'E':
            Current_TotalSumOnArr = 12;
            break;
        case 'F':
            Current_TotalSumOnArr = 13;
            break;
        case 'I':
            Current_TotalSumOnArr = 14;
            break;
        case 'S':
            Current_TotalSumOnArr = 15;
            break;
        case '?':
            Current_TotalSumOnArr = 16;
            break;
        default:
            Current_TotalSumOnArr = 10;
            break;
        }
        for (size_t DigitalSegment_WriteIterator = ZERO_INT; DigitalSegment_WriteIterator <= Shifter_595N.getDataWidth() - SEG_INDEX_DP_EXEMPT; DigitalSegment_WriteIterator++)
        {
            Shifter_595N.set(DigitalSegment_WriteIterator, SingleSegment_Values[Current_TotalSumOnArr][DigitalSegment_WriteIterator]);
        }
        Shifter_595N.write();
    }
    if (SketchTime_IntervalHit(1000))
    {
        if (Current_DecSwitch != LastSave_DecSwitch)
        {
            Shifter_595N.set(Shifter_595N.getDataWidth() - ARR_INDEX_OFFSET, Current_DecSwitch);
            Shifter_595N.write();
            switch (Current_DecSwitch)
            {
            case 0:
                LastSave_DecSwitch = Current_DecSwitch;
                Current_DecSwitch = 1;
                break;
            case 1:
                LastSave_DecSwitch = Current_DecSwitch;
                Current_DecSwitch = ZERO_INT;
                break;
            }
        }
    }
}

// Device Communication Section - Set of Functions that needs report of Serial.
// Device Communications Status Display - Max of 14 Words

static void SerialComms_Init_POST()
{
    ;
}

static void Serial_isCommsAlive()
{
    ;
}

static void SerialHost_SendToReceiver(const char AT_CommandGiven)
{
    ;
}

static void SerialHost_VerifyData()
{
    ;
}

//static void SerialHost_

//RTC Module Dedicated Functions
//static void RTC_GetCurrentTime

static void RTC_InitializeObject()
{
    //Time RTC_Object =
    ;
}

static void RTC_PauseFunction(bool TruthValue)
{
    RTCModule.halt(TruthValue);
}
static void RTC_WriteProtection(bool TruthValue)
{
    RTCModule.writeProtect(TruthValue);
}

static void RTC_PrototypeInit()
{
    Time CheckTime = RTCModule.time();
    if (CheckTime.mon == ZERO_INT0)
    {
        Time RTC_DataContainer(2019, 6, 14, 12, 00, 30, Time::kThursday);
        RTCModule.time(RTC_DataContainer);
    }
}

static void RTC_ManualCorrection()
{
    ;
}

static void RTC_CorrectionDifference()
{
    ;
    /*
if 
else
 */
}
static void RTC_Replace_GetCorrectTimeQuery()
{
    // Add Variables for this arguments per each.
    //Time RTC_Object(2019, 6, 12, 10, 32, 0, Time::kWednesday);
    ;
}

static void RTC_Display_GetChipCurrentTime()
{
    //if (SketchTime_IntervalHit(1000))
    //{
    Time RTC_DataContainer = RTCModule.time();
    String_BeautifyRTC(LCD_I2C, print, "%02d/%02d %02d:%02d:%02d", RTC_DataContainer.mon, RTC_DataContainer.date, RTC_DataContainer.hr, RTC_DataContainer.min, RTC_DataContainer.sec);
    //}
}