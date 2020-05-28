/* =============================================================================
!  Author:          Janrey "CodexLink" Licas | https://github.com/CodexLink
*  Email:           janjan.licas@gmail.com
##  FileName:        GeMCore_TypeDecl.h
##  Description:     Extra Definitions for Type Shortened Data Types and Name References.
!  Version:          0.0.1
*  History:         05/27/2020 - First File Init
============================================================================= */

#pragma once

#include <inttypes.h>

// # Type Definitions
typedef uint8_t byte_p;
typedef int8_t byte_i;
typedef int16_t int_s;
typedef int64_t int_ll;
typedef const struct struct_c;

// # Constant Expressions at Compile Time
constexpr byte_p FUNC_SUCCESS = 0;
constexpr byte_p FUNC_FAILED = 69;
constexpr byte_p INTERNAL_ERR = -1; // ! Unconfirmed...

constexpr byte_p OFFSET_BY_ONE = 1;
constexpr byte_p INCREMENT_BY_ONE = 1;