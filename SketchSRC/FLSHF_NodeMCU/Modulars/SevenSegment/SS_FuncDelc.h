/* =============================================================================
!  Author:          Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:           janjan.licas@gmail.com
##  FileName:        SS_FuncDecl.h
##  Description:     Function Declarations for EEPROM Management of NodeMCU.
!  Version:          0.0.1
*  History:         05/16/2020 - First File Init
============================================================================= */

/*
# @technical
! This does not uses ShiftOut Custom Library with Daisy Chain Support nor Using Internal Function Version of ShiftOut of Arduino!
* Certain Conditions were not! One of them is being able to overwrriten MSB (Last Part / 8th).
* When in fact it was being used as blinking only state. Indication of running sketch.
*/

#pragma once

#include <inttypes.h>
#include <SoftwareSerial.h> // ! Use for Self-Library Debugging Usage!

class SevenSegment : (public, public)
{
    enum SS_STATUS
    {
        SS_WORKING,
        SS_NOT_WORKING,
        SS_ON,
        SS_OFF,
        SS_LOADING_STATE,
        SS_ON_DISP_SYS_CODE,
        SS_UNKNOWN_MODE,
        SS_ON_ITER,
    };

    // @o | This could be done in boolean in function. Further inspection of how enum accessibility is required.
    // ! If unable then proceed with boolean parameter on that function.
    enum BIT_SHIFT_DIRECTION // # Usable by inline void shiftData() __attribute__((always_inlined))
    {
        MSB,
        LSB,
    };

    // @o | Indicator State Usable for Seven Segment Functionaliyt.
    enum SS_TOGGLEABLES // # Usable by void Toggle_SSDisplay();
    {
        SS_ACTIVATE_LEDS,
        SS_DEACTIVATE_LEDS,
        SS_ACTIVATE_DP_BLINKING,
        SS_DEACTIVATE_DP_BLINKING
    };

    // # Animation Options for Use | Usable within Loading Scope or Overriden POST Success State.
    enum SS_ANIM_SEQUENCE
    {
        SS_LOADING_CCW_W_DEC_BLINK,
        SS_LOADING_CW_W_DEC_BLINK,
        SS_LOADING_CW,
        SS_LOADING_CCW,
        SS_OK_HAND, // ! Keep in mind, this animation might be quite complex. Go for 0 sequence, and on 3 middle bars from bottom to top.
    };

    // ! Struct Containing Set of Values To Display and Manipulate Later On.
    struct SS_DEBUG_STATES
    {
        uint8_t SS_STATE;         // # Value must be Enum.
        uint8_t SS_LEDS[7];       // # Shows Whole Array Which One is On and Off.
        int16_t SS_RATE_ITER = 0; // # Shows Refresh Count | For Debugging.
    } SS_STATE_DEBUGGER;

    struct SS_DISP_ARRAY
    {
        static constexpr uint8_t SS_LED_AVAILABLE = 7; // ! This does not include DP (8th). Since DP is manageable by different functions.
        // # SS Whole Number | Here's the Physical Mapping    = {a, b, c, d, e, f, g};
        uint8_t SS_DISP_NUM_1[SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 0, 0};
        uint8_t SS_DISP_NUM_2[SS_LED_AVAILABLE] = {1, 1, 0, 1, 1, 0, 1};
        uint8_t SS_DISP_NUM_3[SS_LED_AVAILABLE] = {1, 1, 1, 1, 0, 0, 0};
        uint8_t SS_DISP_NUM_4[SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 1, 1};
        uint8_t SS_DISP_NUM_5[SS_LED_AVAILABLE] = {1, 0, 1, 1, 0, 1, 1};
        uint8_t SS_DISP_NUM_6[SS_LED_AVAILABLE] = {1, 0, 1, 1, 1, 1, 1};
        uint8_t SS_DISP_NUM_7[SS_LED_AVAILABLE] = {1, 1, 1, 1, 0, 0, 0};
        uint8_t SS_DISP_NUM_8[SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 1};
        uint8_t SS_DISP_NUM_9[SS_LED_AVAILABLE] = {1, 1, 1, 0, 0, 1, 0};
        uint8_t SS_DISP_NUM_0[SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 0};

        // # SS Okay Hand Sequence | Here's the Physical Mapping          = {a, b, c, d, e, f, g};
        uint8_t SS_DISP_OK_HAND_SEQ_BEGIN[SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 0};
        uint8_t SS_DISP_OK_HAND_SEQ_FIRST[SS_LED_AVAILABLE] = {0, 0, 0, 1, 0, 0, 0};
        uint8_t SS_DISP_OK_HAND_SEQ_SECOND[SS_LED_AVAILABLE] = {0, 0, 0, 0, 0, 0, 1};
        uint8_t SS_DISP_OK_HAND_SEQ_FINAL[SS_LED_AVAILABLE] = {1, 0, 0, 0, 0, 0, 0};

        // # SS System Code Binary Array | Here's the Physical Mapping = {a, b, c, d, e, f, g, dp};
        uint8_t SS_DISP_INITIALIZATION[SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 0, 0};
        uint8_t SS_DISP_GET_REQ[SS_LED_AVAILABLE] = {1, 0, 1, 1, 1, 1, 1};
        uint8_t SS_DISP_POST_REQ[SS_LED_AVAILABLE] = {1, 1, 0, 0, 1, 1, 1};
        uint8_t SS_DISP_UNKNOWN[SS_LED_AVAILABLE] = {0, 0, 0, 0, 0, 0, 1};
        uint8_t SS_DISP_SAVE_DATA[SS_LED_AVAILABLE] = {1, 0, 1, 1, 0, 1, 1};
        uint8_t SS_DISP_ACCESS_DETECT[SS_LED_AVAILABLE] = {1, 1, 1, 0, 1, 1, 1};
        uint8_t SS_DISP_ERROR[SS_LED_AVAILABLE] = {1, 0, 0, 1, 1, 1, 1};
    } SS_DISPLAYS;

    // #  GENERAL PIN INDICATION |> {-1: GND | 0: VCC}.
    const struct DEMUX_CONFIG_PIN
    {
        uint8_t DATA_PIN = 0;
        uint8_t CLCK_PIN = 0;
        uint8_t LATCH_PIN = 0;
        uint8_t OE_PIN = -1; // * Just For Reference | Controllable, But not preferred at this kind of situation.
        uint8_t MR_PIN = 0;  // * Just For Reference | Controllable, But not preferred at this kind of situation.
    } DEMUX_REQUIRED_PINS;

    // ! Possibly Default Settings. Might check.
    // @todo | Double Check Sketch on this particular setting.
    static constexpr uint16_t TIME_TRANSISTION = 2000;
    static constexpr uint16_t LOADING_BAR_TRANSISTION = 10000;

private:
    template <typename Des_DType = uint8_t> // # Constricts and Sets Desirable Default Argument. Further Assert Check Will Be Implmeneted.
    // ! @todo | Restrict Data Type for string and uint8_t | Use Assert!

    // # General Functions For Use.
    void ss_ReadUpdate(Des_DType rData);                            // ! Reads Any Value<string, > and Interpret It. Interpretation Application is done by ssUpdateDisplay().
    inline void ss_UpdateDisplay() __attribute__((always_inlined)); //# Launched by SevenSegment::ssReadUpdate | Requires Array of SS_DISP...

    // # DEMUX Functions
    inline void clearData() __attribute__((always_inlined));                              // # Primary Function, For use of Master Reclear Data on Low (~MR) Pin.
    inline void clearDataAlt() __attribute__((always_inlined));                           // # Secondary Function, For General Purpose Only! Not Designed for Master Reclear on Low (~MR) Pin!
    inline void releaseData() __attribute__((always_inlined));                            // # For use of Latch (ST_CP) Pin Only!
    inline void setData() __attribute__((always_inlined));                                // # For use of Data Pin (SD / DS) Pin Only!
    inline void shiftData(BIT_SHIFT_DIRECTION direction) __attribute__((always_inlined)); // # Shifts Data Based from LSB or MSB Output.
    inline void toggleIC() __attribute__((always_inlined));                               // # For use of Output Enable on Low (~OE) Pin Only!

    // # Serial / Debugging Functions | Virtual Function is Usable Here. | SerialDisplayOnce Specialized Please.
    void setErrorCode();
    void printErrorCode();
    void printMsg();

public:
    SevenSegment(bool SHOULD_DEBUG = false, bool INLINE_SERIAL_INIT = false); // ! Constructor.
    void(String Message);
    bool Update_SSDisplay(uint8_t);                 // # Returns false | true, FUNC_SUCCESS else FUNC_FAILED. Subset of DEMUX Functions will be used in this function.
    void Toggle_SSDisplay(SS_TOGGLEABLES ledState); // # Returns Nothing. This function is a toggle function! Toggle each feature by each call. Refer to enum SS_TOGGLEABLES.
};