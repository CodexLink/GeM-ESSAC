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
#include "..\Cores\GEMCore\GeMCore_TypeDefs.h"

class SevenSegment
{
    enum SS_STATUS
    {
        SS_WORKING,
        SS_NOT_WORKING,
        SS_ALL_ON,
        SS_ALL_OFF,
        SS_LOADING_STATE,
        SS_ON_DISP_SYS_CODE,
        SS_UNKNOWN_MODE,
        SS_ON_ITER,

    };

    // #  GENERAL PIN INDICATION |> {-1: GND | 0: VCC}.
    const struct DEMUX_CONFIG_PIN
    {
        byte_p DATA_PIN;
        byte_p CLCK_PIN;
        byte_p LATCH_PIN;
        byte_p OE_PIN = -1; // * Just For Reference | Controllable, But not preferred at this point.
        byte_p MR_PIN = 0;  // * Just For Reference | Controllable, But not preferred at this point.
    };

    struct SS_PROPS
    {
        static byte_p SS_STATE; // # Value must be Enum.
        int_s SS_RATE_ITER;     // # Shows Refresh Count | For Debugging.
    };

private:
    template <typename T = byte_p>
    // ! @todo | Restrict Data Type for string and byte_p
    void ssReadUpdate(T as);       // ! Reads Any Value<string, > and Interpret It. Interpretation Application is done by ssUpdateDisplay().
    inline void ssUpdateDisplay(); //# Launched by SevenSegment::ssReadUpdate.
public:
    SevenSegment(byte_p SHOULD_DEBUG, SS_STATUS OVERRIDE_DISP_ON_POST = SS_STATUS::SS_ALL_OFF);
    byte_p Update_SSDisplay(byte_p); // # Returns 0 | 1 for FUNC_SUCCESS | FUNC_FAILED.
};