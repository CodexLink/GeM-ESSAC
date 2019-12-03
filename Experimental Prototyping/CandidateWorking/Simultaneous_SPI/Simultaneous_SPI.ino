// ! Experimental Prototyping |> Simulataneous I2C Sensor Report
// ! By Janrey Licas
// Created On: <Untracked, Somewhere In Between September and November>

/*  * @info => This sketch simultaneously reports sensors status.
    * @includes => CCS811 and SHT3X
    * @extras => Added PIR Mini Motion Sensor
    ! |> Why?
    *    - Since PIR Mini Motion Sensor Reports by Using Digital Read without
    *    any use of I2C interface. I have to include it to ensure that we are all focused
    *    on Sensor Data Reports.

    ! Note:
    *   Keep in mind, the code added here will be used in the final sketch. We keep things here
    *   formalized in a way of outputting proper styling data in the serial monitor.

    ! TECHNICAL NOTE:
    *   This sketch uses EEPROM of the Arduino to store data. From here, I just to want to say that we will have
    *   some declared spaces for the use of RFID and SENSOR_DATA Structure.
*/
#include "Adafruit_GFX.h"
#include "MFRC522.h"
#include "TFT_ILI9163C.h"

#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define RST_PIN 8
#define SS_PIN 53

TFT_ILI9163C TFTScreen(39, 38); // Other Device Class / Objects Definition
MFRC522 RFIDCmpnt(SS_PIN, RST_PIN);


// ! Im guessing 4 octets here.
uint8_t ReadCard_Container[4] = NULL;
uint8_t MasterCard_Container[4] = NULL;


void setup()
{
    Serial.begin(11500);
    SPI.begin();
    MFRC522.PCD_Init();
    TFTScreen.begin();
    TFTScreen.clearScreen();
    showsetup_TFT();
    MFRC522.PCD_SetAntennaGain(MFRC522.RxGain_max);
}

void loop()
{
  //a single pixel
  TFTScreen.drawPixel(TFTScreen.width()/2, TFTScreen.height()/2, GREEN);
  delay(500);

  // line draw test
  testlines(YELLOW);
  delay(500);

  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);

  testdrawrects(GREEN);
  delay(1000);

  testfillrects(BLUE, YELLOW);
  delay(1000);

  randomRect(0);
  delay(100);
  randomCircles(0);
  delay(100);
  randomLines();
  delay(100);
  randomPoints();
  delay(500);

  TFTScreen.clearScreen();
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

}

void showsetup_TFT()
{
    digitalWrite(8,HIGH);
    delay(1000);
    digitalWrite(8,LOW);
    TFTScreen.clearScreen();
    TFTScreen.setCursor(0, 0);
    TFTScreen.setTextSize(1);
    TFTScreen.setTextColor(YELLOW);
    TFTScreen.print("---------------------");
    TFTScreen.setCursor(TFTScreen.width() / 6, 15);
    TFTScreen.setTextSize(2);
    TFTScreen.setTextColor(YELLOW);
    TFTScreen.print("IoTMesC");

    TFTScreen.setTextSize(1);
    TFTScreen.setTextColor(CYAN);

    TFTScreen.setCursor(TFTScreen.width() / 12, 40);
    TFTScreen.print("Internet of Things");
    TFTScreen.setCursor(TFTScreen.width() / 9, 50);
    TFTScreen.print("Multi - Essential");
    TFTScreen.setCursor(TFTScreen.width() / 12, 60);
    TFTScreen.print("Sensing Components");
    TFTScreen.setTextColor(RED);
    TFTScreen.println(" ");
    TFTScreen.setCursor(TFTScreen.width() / 7, 75);
    TFTScreen.print("Version Unknown");
    TFTScreen.setCursor(TFTScreen.width() / 4, 90);
    TFTScreen.setTextColor(YELLOW);
    TFTScreen.print("Created by");
    TFTScreen.setCursor(TFTScreen.width() / 24, 100);
    TFTScreen.print("Janrey | 'CodexLink'");
    TFTScreen.setTextColor(YELLOW);
    TFTScreen.setTextSize(1);
    TFTScreen.setCursor(0, TFTScreen.height());
    TFTScreen.println(" ");
    TFTScreen.println(" ");
    TFTScreen.print("---------------------");
}

// ! Incomplete
inline bool isMasterRFIDExist()
{
    // ! LET ADDRESS 1 BE RFID_EEPROM_HEADER
    // ! LET ALL BITS ON INDICATION OF MASTER CARD EXIST
    return EEPROM.read(1) == 255 ? 1 : 0;
}

inline uint8_t cout_RFIDRecords()
{
    // ! Do something about it.
    //return EEPROM.read();
}


// After initialization, we should be trying to put the UID of master to a variable to be use later.
inline bool assignVar_ofMasterRFID()
{
    //! NEEDS A VARIABLE FOR THIS FUNCTION
}

// * Stores Scanned Card to ReadCard_Container.
bool RFID_ScanCard()
{
    if (MFRC522.PICC_IsNewCardPresent() || MFRC522.PICC_ReadCardSerial())
    {
        // ! ADD SOME SERIAL OUTPUT HERE.
        for (uint8_t ReadUIDByte = 0; ReadUIDByte < 4; ReadUIDByte++)
        {
            ReadCard_Container[ReadUIDByte] = MFRC522.uid.uidByte[ReadUIDByte];
        }
        //Serial.print("Card Scanned has a UID of |> ");
        //Serial.println(ReadCard_Container[ReadUIDByte], HEX);
        MFRC522.PICC_HaltA();
        return 1;
    }
    else
    {
        // Serial.print("None Scanned.")
        return 0;
    }
}

inline void MasterRFID_Add()
{
    // * This function will run after getting any scanned ID.
    // ! WE STORE BESIDES RFID_EEPROM_HEADER, WHICH WE STORE AT ADDRESS 2
    // ! We define last octet at address + 3. Therefore. start from 0 to 4. Let 4 be the last octet UID
    for (uint8_t MasterADDR = 0; MasterADDR < 4 MasterADDR++)
    {
        // OFFSET BY 1 WHICH WE START AT ADDRESS 2.
        EEPROM.write(1 + MasterADDR, )//Placeholder_Variable)
    }
    EEPROM.write(1, 255);
    return;
}

inline void EEPROM_WipeData()
{
    uint16_t EPPROMBits = NULL;
    while (!EEPROM.read(EEPROMBits) || EEPROMBits < EEPROM.length())
    {
        EEPROM.write(EEPROMBits, NULL);
        EEPROMBits++;
    }
    // ! LET ADDRESS 1 BE RFID_EEPROM_HEADER
    EEPROM.write(1, NULL);
    return;
}

inline void RFID_MasterCardEqual(uint8_t masterCardHolder, uint8_t scannedCard)
{

}

void ()
{

}
void ()
{

}
void ()
{

}
void ()
{

}
/*
    * The functions below are from the bigtest.ino sketch from RFID repo. This was meant to be added at the moment.
    * Because I don't have the time to design all the screen for each proposed that I want to my system.
    * We let it like that for a moment and try to work like we are multithreading here are at this point.
    ! It means I want to scan my RFID cards while I'm updating the screen. Okay?
*/
void testlines(uint16_t color) {
  TFTScreen.clearScreen();
  for (int16_t x=0; x < TFTScreen.width()-1; x+=6) {
    TFTScreen.drawLine(0, 0, x, TFTScreen.height()-1, color);
  }
  for (int16_t y=0; y < TFTScreen.height()-1; y+=6) {
    TFTScreen.drawLine(0, 0, TFTScreen.width()-1, y, color);
  }
  TFTScreen.clearScreen();
  for (int16_t x=0; x < TFTScreen.width()-1; x+=6) {
    TFTScreen.drawLine(TFTScreen.width()-1, 0, x, TFTScreen.height()-1, color);
  }
  for (int16_t y=0; y < TFTScreen.height()-1; y+=6) {
    TFTScreen.drawLine(TFTScreen.width()-1, 0, 0, y, color);
  }

  TFTScreen.clearScreen();
  for (int16_t x=0; x < TFTScreen.width()-1; x+=6) {
    TFTScreen.drawLine(0, TFTScreen.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < TFTScreen.height()-1; y+=6) {
    TFTScreen.drawLine(0, TFTScreen.height()-1, TFTScreen.width()-1, y, color);
  }
  TFTScreen.clearScreen();
  for (int16_t x=0; x < TFTScreen.width()-1; x+=6) {
    TFTScreen.drawLine(TFTScreen.width()-1, TFTScreen.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < TFTScreen.height()-1; y+=6) {
    TFTScreen.drawLine(TFTScreen.width()-1, TFTScreen.height()-1, 0, y, color);
  }
  delay(500);
}


void testdrawtext(char *text, uint16_t color) {
  TFTScreen.setTextSize(1);
  TFTScreen.setTextColor(WHITE);
  TFTScreen.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    TFTScreen.write(i);
    if ((i > 0) && (i % 21 == 0))
      TFTScreen.println();
  }
}

void testfastlines(uint16_t color1, uint16_t color2) {
  TFTScreen.clearScreen();
  for (int16_t y=0; y < TFTScreen.height()-1; y+=5) {
    TFTScreen.drawFastHLine(0, y, TFTScreen.width()-1, color1);
  }
  for (int16_t x=0; x < TFTScreen.width()-1; x+=5) {
    TFTScreen.drawFastVLine(x, 0, TFTScreen.height()-1, color2);
  }
}

void testdrawrects(uint16_t color) {
  TFTScreen.clearScreen();
  for (int16_t x=0; x < TFTScreen.height()-1; x+=6) {
    TFTScreen.drawRect((TFTScreen.width()-1)/2 -x/2, (TFTScreen.height()-1)/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  TFTScreen.clearScreen();
  for (int16_t x=TFTScreen.height()-1; x > 6; x-=6) {
    TFTScreen.fillRect((TFTScreen.width()-1)/2 -x/2, (TFTScreen.height()-1)/2 -x/2 , x, x, color1);
    TFTScreen.drawRect((TFTScreen.width()-1)/2 -x/2, (TFTScreen.height()-1)/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < TFTScreen.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < TFTScreen.height()-1; y+=radius*2) {
      TFTScreen.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < (TFTScreen.width()-1)+radius; x+=radius*2) {
    for (int16_t y=0; y < (TFTScreen.height())-1+radius; y+=radius*2) {
      TFTScreen.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  TFTScreen.clearScreen();
  int color = 0xF800;
  int t;
  int w = TFTScreen.width()/2;
  int x = TFTScreen.height();
  int y = 0;
  int z = TFTScreen.width();
  for(t = 0 ; t <= 15; t+=1) {
    TFTScreen.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  TFTScreen.clearScreen();
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = TFTScreen.width();
    int h = TFTScreen.height();
    for(i = 0 ; i <= 24; i+=1) {
      TFTScreen.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

  // Max of 21 Char For Solid - Character for Size of 1
  // Max of 10 Char for Size of 2
void tftPrintTest() {

  while(1);
}


void randomRect(bool fill){
  TFTScreen.clearScreen();
  uint8_t k,c;
  for (k = 0; k < 16; k++) {
    for (c = 0; c < 32; c++) {
      uint8_t cx, cy, x, y, w, h;
      //  center
      cx = random(0,TFTScreen.width());
      cy = random(0,TFTScreen.height());
      //  size
      w = random(0,30 + 6);
      h = random(0,20 + 4);
      //  upper-left
      x = cx - w / 2;
      y = cy - h / 2;
      if (x < 0) x = 0;
      if (y < 0) y = 0;
      //  adjust size
      if (x + w > TFTScreen.width()) w = TFTScreen.width() - x;
      if (y + h > TFTScreen.height()) h = TFTScreen.height() - y;
      if (fill){
        TFTScreen.fillRect(x, y, w, h,random(0x0010,0xFFFF));
      }
      else {
        TFTScreen.drawRect(x, y, w, h,random(0x0010,0xFFFF));
      }

    }
    TFTScreen.clearScreen();
  }
}

void randomCircles(bool fill){
  TFTScreen.clearScreen();
  uint8_t k,c;
  for (k = 0; k < TFTScreen.height(); k++) {
    for (c = 0; c < TFTScreen.height()/2; c++) {
      //  coordinates
      uint8_t x = random(0,120 + 3), y = random(0,90 + 2), r = random(0,40 + 1);
      if (x - r <  0) r = x;
      if (x + r > (TFTScreen.width()-1)) r = (TFTScreen.width() - 1) - x;
      if (y - r <  0) r = y;
      if (y + r > (TFTScreen.height()-1)) r = (TFTScreen.height() - 1) - y;
      if (fill){
        TFTScreen.fillCircle(x, y, r,random(0x0010,0xFFFF));
      }
      else {
        TFTScreen.drawCircle(x, y, r,random(0x0010,0xFFFF));
      }
    }
    if (!fill)TFTScreen.clearScreen();
  }
}


void randomLines(){
  TFTScreen.clearScreen();
  uint8_t k,c;
  for (k = 0; k < TFTScreen.height(); k++) {
    for (c = 0; c < TFTScreen.height()/2; c++) {
      uint8_t x1 = random(0,TFTScreen.width()), y1 = random(0,TFTScreen.height()), x2 = random(0,TFTScreen.width()), y2 = random(0,TFTScreen.height());
      TFTScreen.drawLine(x1, y1, x2, y2,random(0x0010,0xFFFF));
    }
    TFTScreen.clearScreen();
  }
}


void randomPoints(){
  TFTScreen.clearScreen();
  int k,c;
  for (k = 0; k < 128; k++) {
    for (c = 0; c < 1000; c++) {
      uint8_t x = random(0,TFTScreen.width()), y = random(0,TFTScreen.height());
      TFTScreen.drawPixel(x, y,random(0x0010,0xFFFF));
    }
    TFTScreen.clearScreen();
  }
}