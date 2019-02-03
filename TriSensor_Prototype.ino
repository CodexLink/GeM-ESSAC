#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <SoftReset.h>
#define DHT22_PIN 4
#define HC165_pLoadPin 5
#define HC165_clockEnablePin 6
#define HC165_dataPin 7
#define HC165_clockPin 8
#define HC595_clockPin 9
#define HC595_latchPin 10
#define HC595_dataPin 11
#define AVR_RX_Equiv 2
#define AVR_TX_Equiv 3
#define MQ135_GasSens A3
SoftwareSerial AVR_ESP_Comms(AVR_RX_Equiv, AVR_TX_Equiv);
#define Node_Stats_Enable 0xCD1200
#define Node_Stats_Disable 0xCD0000
#define CST_CurrentResult 1
#define CST_PreviousResult 2
#define CST_IntervalHit 3
#define CST_UnknownDefaultVal 0420
//#define DEBUG_ENABLED 1
#ifdef DEBUG_ENABLED
#define Serial_DebugPrint(x) (Serial.print(x))
#define Serial_DebugPrintln(x) (Serial.println(x))
Serial.print(F("Hello! You just activated my debugging mode."));
Serial.print(F("All Serial_DebugPrint and PrintLn will display."));
#else
#define Serial_DebugPrint(x)
#define Serial_DebugPrintln(x)
#endif

DHT DHT22_Sens(DHT22_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 20, 4);
ShiftIn<1> Shifter_165N;
ShiftOut<1> Shifter_595N;
const byte border_bot_r[] = {B00001, B00001, B00001, B00001, B00001, B00001, B00001, B11111},
           border_bot_l[] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B11111},
           border_top_l[] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B10000},
           border_top_r[] = {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B00001},
           box_sqr_full_l[] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111},
           box_sqr_full_r[] = {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111},
           sd_brdr_l[] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000},
           sd_brdr_r[] = {B00001, B00001, B00001, B00001, B00001, B00001, B00001, B00001},
           divider_char[] = {B00100, B00100, B00100, B00100, B00100, B00100, B00100, B00100},
           SingleSegment[14][8] = {
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
bool Launch_Opt = false, ClearOnce = false;
short RW_Update = 0, Instance_Current = 0,
      Blink_Byte = 0, LastVal_Trigger_1 = 0,
      LastVal_Trigger_2 = 0, RW_MQ135_GasSensRead = 0,
      MQ135_GasSensRead = 0;
unsigned long SketchTime_Prev = 0;
float RW_DHT22_HumidRead = 0,
      RW_DHT22_TempRead = 0;
String SerialResponse[] = {"[WTC]  ", "[OKY]  "};
byte InstanceChecker[] = {0, 0},
     ESPSerial = 0,
     ShifterWidth_165[] = {0, 0, 0, 0, 0, 0, 0, 0},
     Current_InstanceStore[] = {0, 0, 0},
     Locked_IterateData = 0,
     DataChange_Counter[3] = {0, 0, 0};
void setup()
{
    Serial.begin(9600);
    AVR_ESP_Comms.begin(9600);
    Shifter_165N.begin(HC165_pLoadPin, HC165_clockEnablePin, HC165_dataPin, HC165_clockPin);
    Shifter_595N.begin(HC595_dataPin, HC595_clockPin, HC595_latchPin);
    DHT22_Sens.begin();
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, border_bot_l);
    lcd.createChar(1, border_bot_r);
    lcd.createChar(2, sd_brdr_l);
    lcd.createChar(3, sd_brdr_r);
    lcd.createChar(4, border_top_l);
    lcd.createChar(5, border_top_r);
    lcd.createChar(6, divider_char);
    lcd.setCursor(0, 0);
    lcd.write(4);
    lcd.print(F("    DD-TRISenS    "));
    lcd.write(5);
    lcd.setCursor(0, 1);
    lcd.write(2);
    lcd.print(F("   Sldless Type   "));
    lcd.write(3);
    lcd.setCursor(0, 2);
    lcd.write(2);
    lcd.print(F("  NodeMCU Integ.  "));
    lcd.write(3);
    lcd.setCursor(0, 3);
    lcd.write(0);
    lcd.print(F("   Janrey Licas   "));
    lcd.write(1);
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
    delay(500);
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("[ Mode Selection ]"));
    lcd.setCursor(0, 1);
    lcd.print(F("[1] | Server Mode"));
    lcd.setCursor(0, 2);
    lcd.print(F("[2] | NonF.Sys Mode"));
}
void loop()
{
    Shifter_IO_Check();
    if (Launch_Opt == false)
    {
        if (!ShifterWidth_165[0])
        {
            lcd.noBacklight();
            lcd.clear();
            lcd.backlight();
            AVR_InstanceMode(0);
            Launch_Opt = true;
        }
        else if (!ShifterWidth_165[1])
        {
            lcd.noBacklight();
            lcd.clear();
            lcd.backlight();
            AVR_InstanceMode(1);
            Launch_Opt = true;
        }
    }
    else
    {
        Instance_Change();
        DisplayI2C_OnInstance();
    }
}
void Instance_Change()
{
    if (!ShifterWidth_165[0])
    {
        ShifterWidth_165[0] = 1;
        lcd.clear();
        while (1)
        {
            Shifter_IO_Check();
            lcd.setCursor(0, 0);
            lcd.print(F("[Server & AVR Menu ]"));
            lcd.setCursor(0, 1);
            lcd.print(F("[1] | Reboot AVR"));
            lcd.setCursor(0, 2);
            lcd.print(F("[2] | Reboot NodeMCU"));
            lcd.setCursor(0, 3);
            lcd.print(F("[3] | Stop WiFi Serv"));
            if (!ShifterWidth_165[0])
            {
                soft_restart();
            }
            else if (!ShifterWidth_165[1])
            {
            }
            else
            {
            }
        }
    }
    else if (!ShifterWidth_165[1])
    {
        ;
    }
    else if (!ShifterWidth_165[2])
    {
        lcd.clear();
        NodeMCU_Status();
    }
}
void DisplayI2C_OnInstance()
{
    if (Current_InstanceStore[0])
    {
        lcd.setCursor(0, 0);
        lcd.print(F("Server Instance"));
        lcd.setCursor(0, 1);
        lcd.print(F("SSID: "));
        lcd.setCursor(0, 2);
        lcd.print(F("Connection: "));
        lcd.setCursor(0, 3);
        lcd.print(F("Menu          Status"));
    }
    else if (Current_InstanceStore[1])
    {
        float DHT22_TempRead = DHT22_Sens.readTemperature(),
              DHT22_HumidRead = DHT22_Sens.readHumidity(),
              DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false);
        if (isnan(DHT22_TempRead) || isnan(DHT22_HumidRead) ||
            isnan(DHT22_HtInxRead))
        {
            lcd.setCursor(11, 0);
            lcd.print(F("? ERR"));
            lcd.setCursor(12, 1);
            lcd.print(F("? ERR"));
        }
        MQ135_GasSensRead = analogRead(MQ135_GasSens);
        if (!ShifterWidth_165[4])
        {
            lcd.noBacklight();
            lcd.noDisplay();
        }
        else
        {
            lcd.backlight();
            lcd.display();
        }
        if (!ShifterWidth_165[5])
        {
        }
        lcd.setCursor(0, 0);
        lcd.print(F("[  Temp. "));
        lcd.write(126);
        lcd.print(F(" "));
        if (!ShifterWidth_165[6])
        {
            lcd.setCursor(11, 0);
            lcd.print(char(255));
            lcd.print(char(255));
            lcd.print(F("."));
            lcd.print(char(255));
            lcd.print(char(223));
            lcd.print(F("C  ]"));
        }
        else
        {
            if (RW_DHT22_TempRead != DHT22_TempRead)
            {
                DataChange_Counter[0] = 1;
                RW_DHT22_TempRead = DHT22_TempRead;
            }
            else
            {
                DataChange_Counter[0] = 0;
            }

            lcd.print(DHT22_TempRead, 1);
            lcd.print(char(223));
            lcd.print(F("C  ]"));
        }
        lcd.setCursor(0, 1);
        lcd.print(F("[  Humid. "));
        lcd.write(126);
        lcd.print(F(" "));
        if (!ShifterWidth_165[6])
        {
            lcd.setCursor(12, 1);
            lcd.print(char(255));
            lcd.print(char(255));
            lcd.print(F("."));
            lcd.print(char(255));
            lcd.print(F("%  ]"));
        }
        else
        {
            if (RW_DHT22_HumidRead != DHT22_HumidRead)
            {
                DataChange_Counter[1] = 1;
                RW_DHT22_HumidRead = DHT22_HumidRead;
            }
            else
            {
                DataChange_Counter[1] = 0;
            }

            lcd.print(DHT22_HumidRead, 1);
            lcd.print(F("%  ]"));
        }
        lcd.setCursor(0, 2);
        lcd.print(F("[  AirQ. "));
        lcd.write(126);
        lcd.print(F(" "));
        if (!ShifterWidth_165[7])
        {
            lcd.setCursor(11, 2);
            lcd.print(char(255));
            lcd.print(char(255));
            lcd.print(char(255));
            lcd.print(F("ppm  ]"));
        }
        else
        {
            if (RW_MQ135_GasSensRead != MQ135_GasSensRead)
            {
                DataChange_Counter[2] = 1;
                RW_MQ135_GasSensRead = MQ135_GasSensRead;
            }
            else
            {
                DataChange_Counter[2] = 0;
            }
            lcd.print(MQ135_GasSensRead, DEC);
            lcd.print(F("ppm  ]"));
        }
        lcd.setCursor(0, 3);
        lcd.print(F("Menu  SensInfo  WiFi"));
        DigitalSegment_Write(1);
        delay(50);
    }
}
void NodeMCU_Status()
{
    while (1)
    {
        Shifter_IO_Check();
        lcd.setCursor(0, 0);
        lcd.print(F("[NodeMCU Status]"));
        lcd.setCursor(0, 1);
        lcd.print(F("AP"));
        lcd.write(126);
        lcd.print(F("Xiaomi"));
        lcd.setCursor(0, 2);
        lcd.print(F("IP Add"));
        lcd.write(126);
        lcd.setCursor(0, 3);
        lcd.print(F("U: | D: "));
        if (ShifterWidth_165[2])
        {
        }
        else
        {
            break;
        }
    }
    delay(350);
}
byte LoopBack_SerialComms()
{
    short DataCheck;
    if (AVR_ESP_Comms.available() > 0)
    {
        while (1)
        {
            lcd.setCursor(9, 2);
            AVR_ESP_Comms.write(1);
            DataCheck = AVR_ESP_Comms.read();
            if (DataCheck == 1)
            {
                lcd.print(DataCheck, DEC);
                lcd.print(F(" | "));
                lcd.print(SerialResponse[1]);
                return 1;
            }
            else
            {
                lcd.print(DataCheck, DEC);
                lcd.print(F(" | "));
                lcd.print(SerialResponse[0]);
                continue;
            }
        }
    }
    else
    {
        lcd.print(DataCheck, DEC);
        lcd.print(F(" | "));
        lcd.print(SerialResponse[0]);
        return 0;
    }
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
void AVR_InstanceMode(short Instance_Options)
{
    while (ClearOnce != true)
    {
        lcd.setCursor(0, 0);
        lcd.print(F("Serial Connection"));
        lcd.setCursor(0, 1);
        lcd.print(F("Loopback Check"));
        lcd.setCursor(0, 2);
        lcd.print(F("Status "));
        lcd.write(126);
        lcd.print(F(" "));
        if (LoopBack_SerialComms())
        {
            ClearOnce = true;
            delay(900);
            break;
        }
        else
        {
            continue;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Establish Instance"));
    if (Instance_Options == 0)
    {
        lcd.setCursor(0, 1);
        lcd.print(F("To > Server Mode"));
        lcd.setCursor(0, 2);
        lcd.print(F("Please wait..."));
        Current_InstanceStore[Instance_Options] = 1;
    }
    else if (Instance_Options == 1)
    {
        lcd.setCursor(0, 1);
        lcd.print(F("To > F.System Mode"));
        lcd.setCursor(0, 2);
        lcd.print(F("Please wait..."));
        Current_InstanceStore[Instance_Options] = 1;
    }
    delay(500);
    lcd.clear();
}

byte Shifter_IO_Check()
{
    if (Shifter_165N.update())
    {
        Serial_DebugPrint(F("[SHIFTER INFO] Shifter 165N Updated...    -> "));
        for (short BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
        {
            ShifterWidth_165[BitWidth] = Shifter_165N.state(BitWidth);
            Serial_DebugPrint(Shifter_165N.state(BitWidth));
        }
        Serial_DebugPrintln(F(" "));
    }
    else
    {
        Serial_DebugPrint(F("[SHIFTER INFO] Shifter 165N Unchanged... -> "));
        for (short BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
        {
            Serial_DebugPrint(Shifter_165N.state(BitWidth));
        }
    }
    Serial_DebugPrintln(F(" "));
}

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
    for (short i = 0; i < 3; i++)
    {
        Val += DataChange_Counter[i];
    }
    if (Val != LastVal_Trigger_2)
    {
        Serial_DebugPrint(F("[DISPLAY INFO] Single Segment Updated to Val > "));
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
        Serial_DebugPrint(F("[DISPLAY INFO] Single Segment Updated to Val > "));
        Serial_DebugPrintln(Val);
    }
}