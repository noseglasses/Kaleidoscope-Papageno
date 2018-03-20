/* -*- mode: c++ -*-
 * Kaleidoscope-Papageno -- Papageno features for Kaleidoscope
 * Copyright (C) 2017 noseglasses <shinynoseglasses@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

} // This ends region extern "C"

// A note on the use of the __NL__ macro below:
//
//       Preprocessor macro functions can be 
//       hard to debug.
//
//       One approach is to take a look at
//       the preprocessed code to find out 
//       what goes wrong. 
//
//       Unfortunatelly macro replacement cannot deal with newlines.
//       Because of this, code that emerges from excessive macro
//       replacement looks very unreadable due to the absence of 
//       any line breaks.
//
//       To deal with this restriction, comment the
//       definition of the __NL__ macro below, during
//       preprocessor debugging. In that case, the
//       occurrences of __NL__ will not be replaced by
//       and empty string as in the compiled version but
//       will still be present in the preprocessed code.
//       Replace all occurrences of __NL__ in the preprocessed
//       with newlines using a text editor to gain a
//       readable version of the generated code.

#define __NL__
#define __NN__

namespace kaleidoscope {
namespace papageno {
   
// #define PPG_KLS_KEYPOS(ROW, COL, S___)                             
//    S___(ROW, COL)
//
//#define PPG_KLS_INPUT_FROM_KEYPOS_CALL(ROW, COL)                       
//    inputIdFromKeypos(ROW, COL)
//    
// #define PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(KEYPOS_ALIAS)                          
//    PPG_KLS_KEYPOS_INPUT(KEYPOS_ALIAS)
//    
// #define PPG_KLS_INPUT_FROM_ID(ID)                      
//    PPG_KLS_KEYCODE_INPUT(ID)

// The inputs that are defined by Papageno in the firmware sketch
// are mapped to a range of compile time constants whose value starts from 
// zero.
//
// To allow for automatic numbering, we use the __COUNTER__ macro. 
// This macro returns a consecutive set of integer numbers starting from
// zero. It is is important to note that __COUNTER__ is not uniqe for a 
// given include file but for the overall compilation unit. That's 
// why we have to define an offset to make sure that our input ids actually
// start from zero.
//
static constexpr unsigned PPG_Keypos_Input_Offset = __COUNTER__;

#define PPG_KLS_DEFINE_KEYPOS_INPUT_ID(ID, ROW, COL)                           \
__NL__   static constexpr unsigned PPG_KLS_KEYPOS_INPUT(ID)                    \
__NL__               = __COUNTER__ - PPG_Keypos_Input_Offset - 1;

GLS_INPUTS___KEYPOS(PPG_KLS_DEFINE_KEYPOS_INPUT_ID)

// Attention! PPG_Highest_Keypos_Input may be negative in case
// that no keypos inputs are defined
//
static constexpr int16_t PPG_Highest_Keypos_Input
           = (int16_t)(__COUNTER__) - PPG_Keypos_Input_Offset - 2 ;
           
static_assert(PPG_Highest_Keypos_Input >= 0, "PPG_Highest_Keypos_Input negative");

int16_t highestKeyposInputId() {
   return PPG_Highest_Keypos_Input;
}

static constexpr unsigned PPG_Keycode_Input_Offset = __COUNTER__;

#define PPG_KLS_DEFINE_KEYCODE_INPUT_ID(ID)                                    \
__NL__   static constexpr unsigned PPG_KLS_KEYCODE_INPUT(ID)                   \
__NL__      = PPG_Highest_Keypos_Input + (int16_t)(__COUNTER__) - PPG_Keycode_Input_Offset;

GLS_INPUTS___KEYCODE(PPG_KLS_DEFINE_KEYCODE_INPUT_ID)

#define PPG_KLS_DEFINE_COMPLEX_KEYCODE_INPUT_ID(ID, KEYCODE)                   \
            PPG_KLS_DEFINE_KEYCODE_INPUT_ID(ID)

GLS_INPUTS___COMPLEX_KEYCODE(PPG_KLS_DEFINE_COMPLEX_KEYCODE_INPUT_ID)

/* Attention! PPG_Highest_Keycode_Input may be negative in case that
   no keypos and keycode inputs are defined
 */
static constexpr unsigned PPG_Highest_Keycode_Input 
   = PPG_Highest_Keypos_Input
                 + (int16_t)(__COUNTER__) - PPG_Keypos_Input_Offset - 1;

static_assert(PPG_Highest_Keycode_Input <= 255,
   "The number of inputs exceeds the maximum possible number of 255");

// To attach to Kaleidoscope's event handling, we 
// need to be able to determine an input id from a keypos.
//
PPG_Input_Id inputIdFromKeypos(byte row, byte col)
{
   uint16_t id = 256*row + col;

   switch(id) {
   
#     define PPG_KLS_KEYPOS_CASE_LABEL(ID, ROW, COL)                           \
__NL__   case 256*ROW + COL:                                                   \
__NL__      return PPG_KLS_KEYPOS_INPUT(ID);                                   \
__NL__      break;

      GLS_INPUTS___KEYPOS(PPG_KLS_KEYPOS_CASE_LABEL)
   }

   return PPG_KLS_Not_An_Input;
}

// To attach to Kaleidoscope's event handling, we 
// need to be able to determine an input id from a keycode.
//
PPG_Input_Id inputIdFromKeycode(Key keycode)
{
   switch(keycode.raw) {

#     define PPG_KLS_KEYCODE_CASE_LABEL(ID)                                    \
__NL__   case (ID):                                                            \
__NL__      return PPG_KLS_KEYCODE_INPUT(ID);                                  \
__NL__      break;

      GLS_INPUTS___KEYCODE(PPG_KLS_KEYCODE_CASE_LABEL)
      
#     define PPG_KLS_COMPLEX_KEYCODE_CASE_LABEL(ID, KEYCODE)                   \
         PPG_KLS_KEYCODE_CASE_LABEL(ID)
         
      GLS_INPUTS___COMPLEX_KEYCODE(PPG_KLS_COMPLEX_KEYCODE_CASE_LABEL)
   }

   return PPG_KLS_Not_An_Input;
}

PPG_KLS_Keypos ppg_kls_keypos_lookup[] = {

#  define PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY(ID, ROW, COL)                         \
      { .row = ROW, .col = COL },
      
   GLS_INPUTS___KEYPOS(PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY)

   { .row = 0xFF, .col = 0xFFL }
};

Key ppg_kls_keycode_lookup[] = {

#  define PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY(KEYCODE) KEYCODE,
      
   GLS_INPUTS___KEYCODE(PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY)

#  define PPG_KLS_CONVERT_COMPLEX_TO_KEYCODE_ARRAY_ENTRY(ID, KEYCODE) KEYCODE,
      
   GLS_INPUTS___COMPLEX_KEYCODE(PPG_KLS_CONVERT_COMPLEX_TO_KEYCODE_ARRAY_ENTRY)

   (Key){ .raw = (uint16_t)-1}
};

#define PPG_KLS_ADD_ONE(...) + 1

static constexpr unsigned PPG_KLS_N_Inputs = 0
   GLS_INPUTS___KEYPOS(PPG_KLS_ADD_ONE)
   GLS_INPUTS___KEYCODE(PPG_KLS_ADD_ONE)
   GLS_INPUTS___COMPLEX_KEYCODE(PPG_KLS_ADD_ONE)
;

} // namespace papageno
} // namespace kaleidoscope

extern "C" {
