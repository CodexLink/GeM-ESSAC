/*
    ! Variable Segment Display Library
    * Specialized for IoTMesC Project
    * Created by: Janrey "CodexLink" Licas
    * Created on: 11/29/2019 | 15:35

    ! Contains: Function Definitions

    What Does This Library Do?
        -> Utilizes Usage of Digital Segment by Implementing The Following:
            |> Multiplexer Method
                |> Multiplexer Method involves using IC to implement functionality of Digital Segment.
                |> The following supported multiplexer are the following:
                    |> SN74HC595/N
                        |> Requires: Vanilla |> Uses shiftOut() function || ShiftOut Library |> https://github.com/InfectedBytes/ArduinoShiftOut
                    |> 7 Segment Decoder | Coming Soon.

            |> Digital-Dependent Method
                |> U
*/

#include "VariableSegmentDisplay.h"

VariableSegment::VariableSegment()
{
}

VariableSegment::~VariableSegment()
{
}
