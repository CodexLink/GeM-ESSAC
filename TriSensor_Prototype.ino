#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ShiftIn.h>
#include <SoftReset.h>
#define DHT22_PIN 4
#define HC165_pLoadPin 5
#define HC165_pLoadPin 6
#define HC165_pLoadPin 7
#define HC165_pLoadPin 8
#define AVR_RX_Equiv 10
#define AVR_TX_Equiv 11
#define LED_RW_1 12
#define MQ135_GasSens A3
SoftwareSerial AVR_ESP_Comms(AVR_RX_Equiv, AVR_TX_Equiv);
#define Node_Stats_Enable 0xCD1200
#define Node_Stats_Disable 0xCD0000
#define CST_CurrentResult 1
#define CST_PreviousResult 2
#define CST_IntervalHit 3
#define CST_UnknownDefaultVal 0420

DHT DHT22_Sens(DHT22_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 20, 4);
ShiftIn<1> Shifter_165N;

int RW_Update = 0;
String AVR_Response = "...";
String ESP_Response = "...";
byte ESPSerial = 0;
bool Launch_Opt = false;
byte ShifterWidth_165[] = {0, 0, 0, 0, 0, 0, 0, 0};
char LoopBackCommsTest(String Module_Response);
void AVR_InstanceMode(unsigned short Instance_Options);
unsigned long SketchTime_Prev = 0;
unsigned long Current_SketchTimer(long Intervals_Millis, unsigned short Target_Result);
byte Shifter_IO_Check();

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
    Shifter_165N.begin(5, 6, 7, 8);
    DHT22_Sens.begin();
    Serial.begin(115200);
    AVR_ESP_Comms.begin(9600);
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
    lcd.write(2);
    lcd.print("1");
    lcd.write(3);
    lcd.print(" ");
    lcd.write(126);
    lcd.print(" Server Mode");
    lcd.setCursor(0, 2);
    lcd.write(2);
    lcd.print("2");
    lcd.write(3);
    lcd.print(" ");
    lcd.write(126);
    lcd.print(" Instance Mode");
    lcd.setCursor(0, 3);
    lcd.write(2);
    lcd.print("3");
    lcd.write(3);
    lcd.print(" ");
    lcd.write(126);
    lcd.print(" System Mode");
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
            AVR_InstanceMode(1);
        }
        else if (!ShifterWidth_165[1])
        {
            lcd.noBacklight();
            lcd.clear();
            delay(100);
            lcd.backlight();
            lcd.clear();
            Launch_Opt = true;
        }
        else if (!ShifterWidth_165[2])
        {
            lcd.noBacklight();
            lcd.clear();
            AVR_InstanceMode(2);
        }
    }
    else
    {
        TriButton_LCDChange();
    }
}
void TriButton_LCDChange()
{
    if (!ShifterWidth_165[0])
    {
        lcd.clear();
        while (1)
        {
            Shifter_IO_Check();
            delay(300);
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
    else if (!ShifterWidth_165[2])
    {
        lcd.clear();
        NodeMCU_Status();
    }
    else
    {
        MultiSens2LCD_I2C();
    }
}
void MultiSens2LCD_I2C()
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
        //lcd.print(mq135_sensor.getCorrectedPPM(DHT.temperature, DHT.humidity), 0);
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

char LoopBackCommsTest(String Module_Response)
{
    if (Module_Response.equals("AVR"))
    {
        while (AVR_ESP_Comms.available() > 0)
        {
            AVR_ESP_Comms.write("AVR_Comms_Test");
        }
    }
    else if (Module_Response.equals("ESP"))
    {
    }
    else
    {
        return "NULL";
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
                //Serial.print(millis());
                //Serial.print(" - ");
                //Serial.println(SketchTime_Prev);
                continue;
            default:
                return CST_UnknownDefaultVal;
            }
        }
        else
        {
            //Serial.print(millis());
            //Serial.print(" - ");
            //Serial.println(SketchTime_Prev);
        }
    }
}

void AVR_InstanceMode(unsigned short Instance_Options)
{
    lcd.backlight();
    bool ClearOnce = false;
    while (1)
    {
        if (ClearOnce == true)
        {
            LoopBackCommsTest("AVR");
            LoopBackCommsTest("ESP");
            lcd.setCursor(0, 0);
            lcd.print(F("Serial Connection"));
            lcd.setCursor(0, 1);
            lcd.print(F("Waiting Comms..."));
            lcd.write(126);
            lcd.setCursor(0, 2);
            lcd.print(F("AVR Response "));
            lcd.write(126);
            lcd.print(F(" "));
            lcd.print(AVR_Response);
            lcd.setCursor(0, 3);
            lcd.print("ESP Response ");
            lcd.write(126);
            lcd.print(F(" "));
            lcd.print(ESP_Response);
            //while (1)
            //{
            //
            //    if (AVR_ESP_Comms.available())
            //    {
            //        AVR_ESP_Comms.write(AVR_ESP_Comms.read());
            //        break;
            //    }
            //}
        }
        else
        {
            lcd.clear();
            ClearOnce = true;
        }
    }
}

byte Shifter_IO_Check()
{
    if (Shifter_165N.update())
    {
        //Serial.print("Shifter Updated...    -> ");
        for (int i = 0; i < Shifter_165N.getDataWidth(); i++)
        {
            ShifterWidth_165[i] = Shifter_165N.state(i);
            //Serial.print(Shifter_165N.state(i));
        }
        //Serial.println(" ");
    }
    else
    {
        //Serial.print("Shifter Not Changed... -> ");
    }
    for (int i = 0; i < Shifter_165N.getDataWidth(); i++)
    {
        //Serial.print(Shifter_165N.state(i));
    }
    //Serial.println(" ");
}
