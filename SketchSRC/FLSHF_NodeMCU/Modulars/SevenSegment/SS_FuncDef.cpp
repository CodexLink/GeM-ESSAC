/* =============================================================================
!  Author:          Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:           janjan.licas@gmail.com
##  FileName:        SS_FuncDef.h
##  Description:     Function Definition for the Single Seven Segment Display Functionality of NodeMCU.
!  Version:          0.0.1
*  History:         05/16/2020 - First File Init
============================================================================= */

#include "SS_FuncDelc.h"

SevenSegment::SevenSegment(bool SHOULD_DEBUG, bool INLINE_SERIAL_INIT)
{
    if (INLINE_SERIAL_INIT)
        Serial.begin(9600);

    Serial.println(F("Seven Segment | Constructor Initialized w/ Inline Serial Begin (Value of 9600"));

    if (SHOULD_DEBUG)
        Serial.println(F("Seven Segment | Debugging Mode Activated!"));
}

template <typename Des_DType>
void SevenSegment::ssReadUpdate(Des_DType rData)
{
}

inline void SevenSegment::ssUpdateDisplay()
{
}

bool SevenSegment::Update_SSDisplay(uint8_t)
{
}

void SevenSegment::Toggle_SSDisplay(SS_TOGGLEABLES ledState)
{
}