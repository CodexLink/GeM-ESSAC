/*
    ! Variable Segment Display Library
    * Specialized for IoTMesC Project
    * Created by: Janrey "CodexLink" Licas
    * Created on: 11/29/2019 | 15:35

    ! Contains: Function Declarations || Function Declaratives

    What Does THis Library Do?
        -> Utilizes Usage of Digital Segment by Implementing The Following:
            |> Multiplexer Method
                |> Multiplexer Method involves using IC to implement functionality of Digital Segment.
                |> Required Definition: Unknown

                |> The following supported multiplexer are the following:
                    |> SN74HC595/N
                        |> Requires: Vanilla |> Uses shiftOut() function || ShiftOut Library |> https://github.com/InfectedBytes/ArduinoShiftOut
                    |> 7 Segment Decoder | Coming Soon.

            |> Digital-Dependent Method
                |> Utilizes Digital Pins in Arduino or Any MCU-Controllable and Flashable INO Component
                |> Required Definition: Unknown

                        enum SEVEN_SEG_CONSTRAINTS : uint_fast8_t
        {
            DATA_COUNTER_ITER = 6,
            RESERVED_MAX_ITER = 7,
            MAX_CURR_CANDIDATE_VAL = 17
        };


        const uint_fast8_t SevenSegment_Payload[SEVEN_SEG_CONSTRAINTS::MAX_CURR_CANDIDATE_VAL][SEVEN_SEG_CONSTRAINTS::RESERVED_MAX_ITER] = {
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

*/



#ifdef MULTIPLEXER_ENABLE
    #include "ShiftOut.h"
    ShiftOut<1> shift;
#endif

class VariableSD
{
private:
    /* data */
public:
    VariableSD(/* args */);
    ~VariableSD();
};
