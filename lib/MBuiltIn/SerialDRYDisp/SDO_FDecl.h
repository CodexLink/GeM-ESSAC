/* =============================================================================
!  Author:           Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:            janjan.licas@gmail.com
##  FileName:        SDO_FuncDecl.h
##  Description:     Function Declarations for Serial Display Data Once.
!  Version:          0.0.1
*  History:          05/16/2020 - First File Init
============================================================================= */

#pragma once

/*
# @technical

! The main use of this library was to be used in instantiated way.
* Each should have required functions that are declared as pure virtual functions.
! For every instance, Each Name Will Be Different Base On Given Argument 1 from Constructor.

#ifdef DEBUG_ENABLED
#if Cmd_Func == println || print
#define SerialH_Call(Cmd_Func, Params) Serial.Cmd_Func(Params)
#define SerialR_Call(Cmd_Func, Params) Serial1.Cmd_Func(Params)
#else
#define SerialH_Call(Cmd_Func, Params)
#define SerialR_Call(Cmd_Func, Params)
#endif
#endif

*/

#include <inttypes.h>

class SerialDisplay
{
public:
    SerialDisplay(char *LibraryUserName);

private:
    static constexpr uint8_t MAX_MESSAGE_CHAR = 255;
    static constexpr uint8_t MAX_ERR_CODE = 6; // ! Example `CNT-01`.
    struct SerialData
    {
        char currMessage[MAX_MESSAGE_CHAR];
        char currErrCode[MAX_ERR_CODE];
    };

    // # Why Virtuals? We need to make those derived class an opportunity to create own style of strings.
    void setErrorCode();
    virtual void printErrorCode() = 0;
    virtual void printMsg(char msgToDisplay) = 0; // ! Output Data Once When Data IS Almost The Same as Stored Here.

    virtual void forceDataDisplay(); // ! Forces To Display Messages Added
}