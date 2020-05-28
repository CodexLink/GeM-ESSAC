/* =============================================================================
!  Author:          Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:           janjan.licas@gmail.com
##  FileName:        SS_FuncDecl.h
##  Description:     Function Declarations for EEPROM Management of NodeMCU.
!  Version:          0.0.1
*  History:         05/16/2020 - First File Init
============================================================================= */

#pragma once

#include <inttypes.h>
#include "..\Cores\GEMCore\GeMCore_TypeDecl.h"

class SevenSegment
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

    // # Usable by void Toggle_SSDisplay();
    enum SS_TOGGLEABLES
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

    struct SS_PROPS
    {
        constexpr byte_p SS_LED_AVAILABLE = 7; // ! This does not include DP. Since DP is manageable by different functions.
        static byte_p SS_STATE;                // # Value must be Enum.
        static byte_p SS_LEDS[SS_LED_AVAILABLE];
        int_s SS_RATE_ITER = NULL; // # Shows Refresh Count | For Debugging.
    } SS_STATE_PROPS;

    const struct SS_DISP_ARRAY
    {
        // # SS Whole Number | Here's the Physical Mapping    = {a, b, c, d, e, f, g};
        byte_p SS_DISP_NUM_1[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 0, 0};
        byte_p SS_DISP_NUM_2[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 0, 1, 1, 0, 1};
        byte_p SS_DISP_NUM_3[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 1, 0, 0, 0};
        byte_p SS_DISP_NUM_4[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 1, 1};
        byte_p SS_DISP_NUM_5[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 1, 1, 0, 1, 1};
        byte_p SS_DISP_NUM_6[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 1, 1, 1, 1, 1};
        byte_p SS_DISP_NUM_7[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 1, 0, 0, 0};
        byte_p SS_DISP_NUM_8[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 1};
        byte_p SS_DISP_NUM_9[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 0, 0, 1, 0};
        byte_p SS_DISP_NUM_0[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 0};

        // # SS Okay Hand Sequence | Here's the Physical Mapping          = {a, b, c, d, e, f, g};
        byte_p SS_DISP_OK_HAND_SEQ_BEGIN[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 1, 1, 1, 0};
        byte_p SS_DISP_OK_HAND_SEQ_FIRST[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 0, 0, 1, 0, 0, 0};
        byte_p SS_DISP_OK_HAND_SEQ_SECOND[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 0, 0, 0, 0, 0, 1};
        byte_p SS_DISP_OK_HAND_SEQ_FINAL[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 0, 0, 0, 0, 0};

        // # SS System Code Binary Array | Here's the Physical Mapping = {a, b, c, d, e, f, g, dp};
        byte_p SS_DISP_INITIALIZATION[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 1, 1, 0, 0, 0, 0};
        byte_p SS_DISP_GET_REQ[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 1, 1, 1, 1, 1};
        byte_p SS_DISP_POST_REQ[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 0, 0, 1, 1, 1};
        byte_p SS_DISP_UNKNOWN[SS_STATE_PROPS.SS_LED_AVAILABLE] = {0, 0, 0, 0, 0, 0, 1};
        byte_p SS_DISP_SAVE_DATA[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 1, 1, 0, 1, 1};
        byte_p SS_DISP_ACCESS_DETECT[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 1, 1, 0, 1, 1, 1};
        byte_p SS_DISP_ERROR[SS_STATE_PROPS.SS_LED_AVAILABLE] = {1, 0, 0, 1, 1, 1, 1};
    } SS_DISPLAYS;

    // #  GENERAL PIN INDICATION |> {-1: GND | 0: VCC}.
    const struct DEMUX_CONFIG_PIN
    {
        byte_p DATA_PIN;
        byte_p CLCK_PIN;
        byte_p LATCH_PIN;
        byte_p OE_PIN = -1; // * Just For Reference | Controllable, But not preferred at this kind of situation.
        byte_p MR_PIN = 0;  // * Just For Reference | Controllable, But not preferred at this kind of situation.
    } DEMUX_REQUIRED_PINS;

private:
    template <typename Des_DType = byte_p>
    // ! @todo | Restrict Data Type for string and byte_p
    void ssReadUpdate(Des_DType rData); // ! Reads Any Value<string, > and Interpret It. Interpretation Application is done by ssUpdateDisplay().
    inline void ssUpdateDisplay();      //# Launched by SevenSegment::ssReadUpdate | Requires Array of SS_DISP...

public:
    SevenSegment(byte_p SHOULD_DEBUG = false, byte_p INLINE_SERIAL_INIT = false);
    bool Update_SSDisplay(byte_p);                  // # Returns 0 | 1 for FUNC_SUCCESS | FUNC_FAILED.
    void Toggle_SSDisplay(SS_TOGGLEABLES ledState); // # Returns Nothing. This function is a toggle function! Toggle each feature by each call. Refer to enum SS_TOGGLEABLES.
};