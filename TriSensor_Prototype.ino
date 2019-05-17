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
//#define HC165_pLoadPin 22
//#define HC165_clockEnablePin 24
//#define HC165_dataPin 26
//#define HC165_clockPin 28
#define DHT22_PIN 22
//#define SDPT_SingleLCDSwitch 23
//#define Button_GeneralUse 24
#define HC595_dataPin 25
#define HC595_latchPin 26
#define HC595_clockPin 27
#define MFRC22_RST_PIN 49
#define MFRC22_SS_PIN 53

#define MQ135_GasSens A0
//#define AVR_RX_Equiv 40
//#define AVR_TX_Equiv 3
//SoftwareSerial Serial1(AVR_RX_Equiv, AVR_TX_Equiv);
#define Node_Stats_Enable 0xCD1200
#define Node_Stats_Disable 0xCD0000
#define CST_CurrentResult 1
#define CST_PreviousResult 2
#define CST_IntervalHit 3
#define CST_UnknownDefaultVal 0420

#define SerialPrimary Serial
#define SerialSecondary Serial1
//#define DEBUG_ENABLED 1
#ifdef DEBUG_ENABLED
#define Serial_DebugPrint(x) (SerialPrimary.print(x))
#define Serial_DebugPrintln(x) (SerialPrimary.println(x))
//Serial_DebugPrint(F("Hello! You just activated my debugging mode."));
//Serial_DebugPrint(F("All Serial_DebugPrint and PrintLn will display."));
#else
#define Serial_DebugPrint(x)
#define Serial_DebugPrintln(x)
#endif

DHT DHT22_Sens(DHT22_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 mfrc522(MFRC22_SS_PIN, MFRC22_RST_PIN);
MQ135 MQ135_Sens = MQ135(MQ135_GasSens);
//ShiftIn<1> Shifter_165N;
ShiftOut<1> Shifter_595N;
const byte SingleSegment[14][8] = {
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
    {0, 0, 0, 0, 0, 0, 0, 0}, // F
};
const byte BatteryChar_Payload[6][8]{
    {B01110, B01110, B11111, B11111, B11111, B11111, B11111, B11111}, // 80-100
    {B01110, B01010, B10001, B11111, B11111, B11111, B11111, B11111}, // 60 - 79
    {B01110, B01010, B10001, B10001, B11111, B11111, B11111, B11111}, // 40 - 59
    {B01110, B01010, B10001, B10001, B10001, B11111, B11111, B11111}, // 20 - 39
    {B01110, B01010, B10001, B10001, B10001, B10001, B11111, B11111}, // 1 - 19
    {B01110, B01010, B10001, B10001, B10001, B10001, B10001, B11111}, // Unknown
    };
bool RFID_Authorization = true;
short Instance_Current = 0,
      Blink_Byte = 0,
      LastVal_Trigger_1 = 0,
      LastVal_Trigger_2 = 0;
unsigned long SketchTime_Prev = 0;
float RW_DHT22_HumidRead = 0,
      RW_DHT22_TempRead = 0,
      RW_DHT22_HtInxRead = 0,
      RW_MQ135_GasSensRead = 0;
byte InstanceChecker[] = {0, 0},
     ESPSerial = 0,
     //ShifterWidth_165[] = {0, 0, 0, 0, 0, 0, 0, 0},
    DataChange_Counter[4] = {0, 0, 0, 0};

short int Battery_PosX = 0, Battery_PosY = 0, BatteryIntDispX = 0, BatteryIntDispX void setup()
{
    SerialPrimary.begin(115200);
    SerialSecondary.begin(115200);
    SPI.begin();        // MFRC522 Hardware uses SPI protocol
    mfrc522.PCD_Init(); // Initialize MFRC522 Hardware
    //Shifter_165N.begin(HC165_pLoadPin, HC165_clockEnablePin, HC165_dataPin, HC165_clockPin);
    Shifter_595N.begin(HC595_dataPin, HC595_clockPin, HC595_latchPin);
    DHT22_Sens.begin();
    //pinMode(SDPT_SingleLCDSwitch, INPUT);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(F("     Welcome!     "));
    lcd.setCursor(0, 1);
    lcd.print(F("    DD-TRISenS    "));
    lcd.setCursor(0, 2);
    lcd.print(F("   Janrey Licas   "));
    lcd.setCursor(0, 3);
    lcd.print(F("  Version 1.1420  "));
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
    delay(500);
    lcd.backlight();
}
void loop()
{
    DisplayI2C_OnInstance();
}
void DisplayI2C_OnInstance()
{
    float DHT22_TempRead = DHT22_Sens.readTemperature(),
          DHT22_HumidRead = DHT22_Sens.readHumidity(),
          DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false),
          //MQ135_GasSensRead = MQ135_Sens.getPPM();
        MQ135_GasSensRead = analogRead(MQ135_GasSens);
    if (digitalRead(SDPT_SingleLCDSwitch) == HIGH)
    {
        lcd.noBacklight();
        lcd.noDisplay();
    }
    else
    {
        lcd.backlight();
        lcd.display();
    }
    if (digitalRead(Button_GeneralUse))
    {
        soft_restart();
    }
    if (RW_DHT22_TempRead != DHT22_TempRead)
    {
        DataChange_Counter[0] = 1;
        RW_DHT22_TempRead = DHT22_TempRead;
    }
    else
    {
        DataChange_Counter[0] = 0;
    }
    if (RW_DHT22_HumidRead != DHT22_HumidRead)
    {
        DataChange_Counter[1] = 1;
        RW_DHT22_HumidRead = DHT22_HumidRead;
    }
    else
    {
        DataChange_Counter[1] = 0;
    }
    if (RW_DHT22_HtInxRead != DHT22_HtInxRead)
    {
        DataChange_Counter[2] = 1;
        RW_DHT22_HtInxRead = DHT22_HtInxRead;
    }
    else
    {
        DataChange_Counter[2] = 0;
    }
    if (RW_MQ135_GasSensRead != MQ135_GasSensRead)
    {
        DataChange_Counter[3] = 1;
        RW_MQ135_GasSensRead = MQ135_GasSensRead;
    }
    else
    {
        DataChange_Counter[3] = 0;
    }
    if (isnan(DHT22_TempRead) || isnan(DHT22_HumidRead) || isnan(DHT22_HtInxRead))
    {
        lcd.setCursor(0, 0);
        lcd.print(F("TE:ERROR"));
        lcd.print(F("  | HU:ERROR"));
        lcd.setCursor(0, 1);
        lcd.print(F("HI:ERROR"));
        lcd.print(F("  | AQ:"));
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print(F("TE:"));
        lcd.print(DHT22_TempRead, 1);
        lcd.print(char(223));
        lcd.print(F("C | HU:"));
        lcd.print(DHT22_HumidRead, 1);
        lcd.print(F("%"));
        lcd.setCursor(0, 1);
        lcd.print(F("HI:"));
        lcd.print(DHT22_HtInxRead, 1);
        lcd.print(char(223));
        lcd.print(F("C | AQ:"));
    }
    if (MQ135_GasSensRead > 999)
    {
        lcd.print(MQ135_GasSensRead, 0);
    }
    else
    {
        lcd.print(MQ135_GasSensRead, 1);
    }

    lcd.setCursor(0, 2);
    lcd.print(F("ST:"));
    //ESP_isSerialOkayToAVR();
    lcd.print(F(" | SE:"));
    if (Serial1.available() > 0)
    {
        lcd.print(Serial1.read(), DEC);
    }
    lcd.setCursor(0, 3);
    //ESP_returnAccess();
    lcd.print("ID:NFC_12 | PW:12356");
    DigitalSegment_Write(1);
    delay(50);
}

unsigned long Current_SketchTimer(long Intervals_Millis, unsigned short Target_Result)
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
                Serial_DebugPrint(millis());
                Serial_DebugPrint(F(" - "));
                Serial_DebugPrintln(SketchTime_Prev);
                return 1;
            default:
                return CST_UnknownDefaultVal;
            }
        }
        else
        {
            Serial_DebugPrint(millis());
            Serial_DebugPrint(F(" - "));
            Serial_DebugPrintln(SketchTime_Prev);
        }
    }
}

//byte Shifter_IO_Check()
//{
//    if (Shifter_165N.update())
//    {
//        Serial_DebugPrint(F("[SHIFTER INFO] Shifter 165N Updated...    -> "));
//        for (short BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
//        {
//            ShifterWidth_165[BitWidth] = Shifter_165N.state(BitWidth);
//            Serial_DebugPrint(Shifter_165N.state(BitWidth));
//        }
//        Serial_DebugPrintln(F(" "));
//    }
//    else
//    {
//        Serial_DebugPrint(F("[SHIFTER INFO] Shifter 165N Unchanged... -> "));
//        for (short BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
//        {
//            Serial_DebugPrint(Shifter_165N.state(BitWidth));
//        }
//    }
//    Serial_DebugPrintln(F(" "));
//}

void DigitalSegment_Write(byte Dec_Trigger)
{
    short Val = 0;
    if (Dec_Trigger != LastVal_Trigger_1)
    {
        switch (Dec_Trigger)
        {
        case 0:
            Shifter_595N.set(Shifter_595N.getDataWidth() - 1, 0);
            Shifter_595N.write();
            LastVal_Trigger_1 = Dec_Trigger;
        case 1:
            Shifter_595N.set(Shifter_595N.getDataWidth() - 1, 1);
            Shifter_595N.write();
            LastVal_Trigger_1 = Dec_Trigger;
        }
    }
    for (short i = 0; i < 4; i++)
    {
        Val += DataChange_Counter[i];
    }
    if (Val != LastVal_Trigger_2)
    {
        Serial_DebugPrint(F("[SEGMENT] Update Value > "));
        Serial_DebugPrintln(Val);
        switch (Val)
        {
        case 0:
            for (short ElementCounter = 0; ElementCounter < Shifter_595N.getDataWidth() - 1; ElementCounter++)
            {
                Shifter_595N.set(ElementCounter, SingleSegment[0][ElementCounter]);
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
                Shifter_595N.set(ElementCounter, SingleSegment[Val][ElementCounter]);
                Shifter_595N.write();
            }
            LastVal_Trigger_2 = Val;
        }
    }
    else
    {
        Serial_DebugPrint(F("[SEGMENT] Update Value > "));
        Serial_DebugPrintln(Val);
    }
}

void CustomCharBattery_LCDWrite(short int CurrentRead_BatteryLevel)
{

    static Battery_LastSaveLevel;
    if (Battery_LastSaveLevel != CurrentRead_BatteryLevel)
    {
        lcd.setCursor(Battery_PosX, Battery_PosY);
        switch (BatteryLevel)
        {
        case 0 ... 19:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 10 ... 19:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 20 ... 29:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 30 ... 39:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 40 ... 49:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 50 ... 59:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 60 ... 69:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 70 ... 79:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

        case 80 ... 89:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

    case 90 ... 100:
        lcd.createChar(0, BatteryChar_Payload[][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);

    default:
        lcd.createChar(0, BatteryChar_Payload[0][18];
        lcd.write(byte(0));
        lcd.setCursor(BatteryIntDispX, Battery_PosY);
        }
    }
}