/* =============================================================================
!  Author:          Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:           janjan.licas@gmail.com
##  FileName:        SevenSegment.ino
##  Description:     Sketch for Use to Debug and Test All Functionalities for the SevenSegment.
!  Version:          0.0.1
*  History:         05/28/2020 - First File Init
============================================================================= */

#include "SS_FuncDelc.h"
#define TIME_TRANSISTION 2000
#define LOADING_BAR_TRANSISTION 10000
SevenSegment ssDriver(true, true);

void displayInit();
void displayWholeNum();
void displaySysCode();

// ! Use Case While Transistioning...
void displayLetBlinkOn();
void displayLetBlinkOff();

void setup()
{
    Serial.println(F("Seven Segment | Functionality Test!"));
    delay(TIME_TRANSISTION);

    Serial.println(F("Seven Segment | Functionality |> Init."));
    Serial.println(F(""));
}

void loop()
{
}

void displayInit()
{
}
void displayWholeNum()
{
}
void displaySysCode()
{
}