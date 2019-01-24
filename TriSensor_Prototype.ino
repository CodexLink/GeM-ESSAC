#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ShiftIn.h>
#include <SoftReset.h>
#define DHT22_PIN 4
#define HC165_pLoadPin 5
#define HC165_clockEnablePin 6
#define HC165_dataPin 7
#define HC165_clockPin 8
#define AVR_RX_Equiv 2
#define AVR_TX_Equiv 3
#define LED_RW_1 12
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
#else
#define Serial_DebugPrint(x)
#define Serial_DebugPrintln(x)
#endif
DHT DHT22_Sens(DHT22_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 20, 4);
ShiftIn<1> Shifter_165N;

short RW_Update = 0, Instance_Current = 0;
String SerialResponse[] = {"[WTC]", "[OKY]"};
byte InstanceChecker[] = {0, 0};
byte ESPSerial = 0;
bool Launch_Opt = false;
bool ClearOnce = false;
byte ShifterWidth_165[] = {0, 0, 0, 0, 0, 0, 0, 0};
byte Current_InstanceStore[] = {0, 0, 0};
byte Locked_IterateData = 0;
unsigned long SketchTime_Prev = 0;
byte border_bot_r[8] = {
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B11111};
byte border_bot_l[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11111};
byte border_top_l[8] = {
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000};
byte border_top_r[8] = {
    B11111,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001};
byte box_sqr_full_l[8] = {
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11111};
byte box_sqr_full_r[8] = {
    B11111,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B11111};
byte sd_brdr_l[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000};
byte sd_brdr_r[8] = {
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001};
byte divider_char[8] = {
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100};
void setup()
{
    Serial.begin(9600);
    AVR_ESP_Comms.begin(9600);
    Shifter_165N.begin(HC165_pLoadPin, HC165_clockEnablePin, HC165_dataPin, HC165_clockPin);
    DHT22_Sens.begin();
    pinMode(LED_RW_1, OUTPUT);
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
    delay(2500);
    lcd.noBacklight();
    lcd.clear();
    delay(500);
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("[ Mode Selection ]");
    lcd.setCursor(0, 1);
    lcd.print("[1] | Server Mode");
    lcd.setCursor(0, 2);
    lcd.print("[2] | NonF.Sys Mode");
    lcd.setCursor(0, 3);
    lcd.print("[3] | F.System Mode");
}
void loop()
{
    Shifter_IO_Check();
    if (Launch_Opt == false)
    {
        lcd.noBacklight();
        lcd.clear();
        lcd.backlight();
        Launch_Opt = true;
        if (!ShifterWidth_165[0])
        {
            AVR_InstanceMode(0);
        }
        else if (!ShifterWidth_165[1])
        {
            AVR_InstanceMode(1);
        }
        else if (!ShifterWidth_165[2])
        {
            AVR_InstanceMode(2);
        }
    }
    else
    {
        Instance_Change();
        DisplayI2C_OnInstance(Instance_Current);
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
        lcd.clear();
        NodeMCU_Status();
    }
    else if (!ShifterWidth_165[2])
    {
        lcd.clear();
        NodeMCU_Status();
    }
}
void DisplayI2C_OnInstance(short Instance_Choice)
{
    int RW_MQ135_GasSensRead = 0, MQ135_GasSensRead = 0;
    float RW_DHT22_HumidRead = 0, RW_DHT22_TempRead = 0,
          DHT22_TempRead = DHT22_Sens.readTemperature(),
          DHT22_HumidRead = DHT22_Sens.readHumidity(),
          DHT22_HtInxRead = DHT22_Sens.computeHeatIndex(DHT22_TempRead, DHT22_HumidRead, false);
    if (isnan(DHT22_TempRead) || isnan(DHT22_HumidRead) || isnan(DHT22_HtInxRead))
    {
        lcd.setCursor(0, 0);
        lcd.print(F("Failed to read from DHT sensor!"));
        lcd.setCursor(0, 1);
        lcd.print(F("Failed to read from DHT sensor!"));
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
        lcd.print(".");
        lcd.print(char(255));
        lcd.print(char(223));
        lcd.print(F("C  ]"));
    }
    else
    {
        if (RW_DHT22_TempRead == DHT22_TempRead)
        {
            digitalWrite(LED_RW_1, LOW);
        }
        else
        {
            digitalWrite(LED_RW_1, HIGH);
            RW_DHT22_TempRead = DHT22_TempRead;
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
        lcd.print(".");
        lcd.print(char(255));
        lcd.print(F("%  ]"));
    }
    else
    {
        if (RW_DHT22_HumidRead == DHT22_HumidRead)
        {
            digitalWrite(LED_RW_1, LOW);
        }
        else
        {
            digitalWrite(LED_RW_1, HIGH);
            RW_DHT22_HumidRead = DHT22_HumidRead;
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
        if (RW_MQ135_GasSensRead == MQ135_GasSensRead)
        {
            digitalWrite(LED_RW_1, LOW);
        }
        else
        {
            digitalWrite(LED_RW_1, HIGH);
            RW_MQ135_GasSensRead = MQ135_GasSensRead;
        }
        lcd.print(MQ135_GasSensRead, DEC);
        lcd.print(F("ppm  ]"));
    }
    lcd.setCursor(0, 3);
    lcd.print("Menu  SensInfo  WiFi");
    delay(50);
}
void NodeMCU_Status()
{
    while (1)
    {
        Shifter_IO_Check();
        lcd.setCursor(0, 0);
        lcd.print("[NodeMCU Status]");
        lcd.setCursor(0, 1);
        lcd.print("AP");
        lcd.write(126);
        lcd.print("Xiaomi");
        lcd.setCursor(0, 2);
        lcd.print("IP Add");
        lcd.write(126);
        //lcd.print(AVR_ESP_Comms.write(WiFi.localIP()));
        lcd.setCursor(0, 3);
        lcd.print("U: | D: ");
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

// CUSTOM FUNCTIONS

int LoopBack_SerialComms()
{
    int DataCheck;
    if (AVR_ESP_Comms.available() > 0)
    {
        while (1)
        {
            lcd.setCursor(9, 3);
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
            if (Target_Result == CST_CurrentResult)
            {
                SketchTime_Prev = 0;
                return millis();
            }
            else if (Target_Result == CST_PreviousResult)
            {
                return SketchTime_Prev;
            }
            else if (Target_Result == CST_IntervalHit)
            {
                Serial_DebugPrint(millis());
                Serial_DebugPrint(F(" - "));
                Serial_DebugPrintln(SketchTime_Prev);
                continue;
            }
            else
            {
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
        lcd.print(F("Wait for LoopBack"));
        lcd.setCursor(0, 2);
        lcd.print(F("AVR & ESP Response"));
        lcd.setCursor(0, 3);
        lcd.print(F("Status "));
        lcd.write(126);
        lcd.print(F(" "));
        if (LoopBack_SerialComms())
        {
            ClearOnce = true;
            delay(5000);
            break;
        }
        else
        {
            continue;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Establish Instance");
    if (Instance_Options == 0)
    {
        lcd.setCursor(0, 1);
        lcd.print("To > Server Mode");
        lcd.setCursor(0, 2);
        lcd.print("Please wait...");
        Current_InstanceStore[Instance_Options - 1] = 1;
    }
    else if (Instance_Options == 1)
    {
        lcd.setCursor(0, 1);
        lcd.print("To > NonF.Sys Mode");
        lcd.setCursor(0, 2);
        lcd.print("Please wait...");
        Current_InstanceStore[Instance_Options - 1] = 1;
    }
    else if (Instance_Options == 2)
    {
        lcd.setCursor(0, 1);
        lcd.print("To > F.System Mode");
        lcd.setCursor(0, 2);
        lcd.print("Please wait...");
        Current_InstanceStore[Instance_Options - 1] = 1;
    }
    delay(1200);
    SetInstance_Mode();
}

byte Shifter_IO_Check()
{
    if (Shifter_165N.update())
    {
        Serial_DebugPrint(F("Shifter Updated...    -> "));
        for (int BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
        {
            ShifterWidth_165[BitWidth] = Shifter_165N.state(BitWidth);
            Serial_DebugPrint(Shifter_165N.state(BitWidth));
        }
        Serial_DebugPrintln(F(" "));
    }
    else
    {
        Serial_DebugPrint(F("Shifter Not Changed... -> "));
        for (int BitWidth = 0; BitWidth < Shifter_165N.getDataWidth(); BitWidth++)
        {
            Serial_DebugPrint(Shifter_165N.state(BitWidth));
        }
    }
    Serial_DebugPrintln(F(" "));
}

void SetInstance_Mode()
{
    short Instance_TrueState = 0;
    if (!Locked_IterateData)
    {
        for (short IterateData = 0; IterateData > 3; IterateData++)
        {
            if (Current_InstanceStore[IterateData] == 1)
            {
                Locked_IterateData = 1;
                Instance_Current = Instance_TrueState;
                break;
            }
            else
            {
                Instance_TrueState++;
            }
        }
    }
    else if (Locked_IterateData)
    {
        DisplayI2C_OnInstance(Instance_Current);
    }
}