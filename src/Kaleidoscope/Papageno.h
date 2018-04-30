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

#include <Kaleidoscope.h>

// avr-gcc compiles files with .c ending with C name mangling
//
extern "C" {
#include "papageno.h"
#include "ppg_settings.h"
}

namespace kaleidoscope {
namespace papageno {
   
typedef struct {
   byte row;
   byte col;
} PPG_KLS_Keypos;

class Papageno : public KaleidoscopePlugin
{
   public:
      
      void begin() final;
      
      void init();
      
      static void setEnabled(bool state);
      static bool getEnabled();
      
      // Some utility functions required by Papageno's API
      //
      static void processKeycode(PPG_Count activation_flags, void *user_data);
      static void processKeypos(PPG_Count activation_flags, void *user_data);
      
      void loop();
      
   private:

      static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
//       static void loopHook(bool is_post_clear);
};

enum { PPG_KLS_Not_An_Input = (PPG_Input_Id)-1 };

// The following extern entities are initialized in Papageno-Initialization.h
//
extern PPG_KLS_Keypos ppg_kls_keypos_lookup[];

extern PPG_Input_Id inputIdFromKeypos(byte row, byte col);

extern int16_t highestKeyposInputId();

extern void time(PPG_Time *time);

extern void timeDifference(PPG_Time time1, PPG_Time time2, PPG_Time *delta);

extern int8_t timeComparison(
                        PPG_Time time1,
                        PPG_Time time2);

} // namespace papageno
} // namespace kaleidoscope

extern kaleidoscope::papageno::Papageno Papageno;
#define PPG_KLS_PREVENT_EXPANSION(S, ...) S##__VA_ARGS__

#define PPG_KLS_TRICAT(S1, S2, S3) S1##S2##S3

#define PPG_KLS_KEYPOS_INPUT(UNIQUE_ID)                                               \
   PPG_KLS_TRICAT(PPG_, UNIQUE_ID, _Keypos_Name)
   
#define PPG_CALLBACK_NO_REPEAT \
   if(activation_flags & PPG_Action_Activation_Flags_Repeated) { return; }
   
#define PPG_CALLBACK_ONLY_ACTIVATION \
   if(!(activation_flags & PPG_Action_Activation_Flags_Active)) { return; }

//##############################################################################
// Definitions for Papageno's Glockenspiel compiler interface
//##############################################################################

#define GLS_ENABLE_INPUTS_LOCAL_INITIALIZATION___KEYPOS

// This macro is used by the Glockenspiel compiled code to initialize
// keypos based inputs. It boils down to the name of a constexpr integer that
// references the input.
//
#define GLS_INPUT_INITIALIZE___KEYPOS(UNIQUE_ID, USER_ID, ROW, COL) \
   kaleidoscope::papageno::PPG_KLS_KEYPOS_INPUT(UNIQUE_ID)
   
// Keycode actions are compile time constant and can thus already be
// assigned when the global static Papageno search tree is initialized.
//
#define GLS_ACTION_INITIALIZE___KEYCODE(...)                                   \
__NL__   {                                                                     \
__NL__      __GLS_DI__(callback)  {                                                \
__NL__         __GLS_DI__(func) (PPG_Action_Callback_Fun)                          \
__NL__                    kaleidoscope::papageno::Papageno::processKeycode,    \
__NL__         __GLS_DI__(user_data) reinterpret_cast<void*>((__VA_ARGS__).raw)    \
__NL__      }                                                                  \
__NL__   } 

#define GLS_ACTION_INITIALIZE___COMPLEX_KEYCODE(UNIQUE_ID, USER_ID, ...)                       \
__NL__   {                                                                     \
__NL__      __GLS_DI__(callback)  {                                                \
__NL__         __GLS_DI__(func) (PPG_Action_Callback_Fun)                          \
__NL__                    kaleidoscope::papageno::Papageno::processKeycode,    \
__NL__         __GLS_DI__(user_data) reinterpret_cast<void*>((__VA_ARGS__).raw)    \
__NL__      }                                                                  \
__NL__   } 

#define GLS_ACTION_INITIALIZE___KEYPOS(UNIQUE_ID, USER_ID, ROW, COL)                           \
__NL__   {                                                                     \
__NL__      __GLS_DI__(callback)  {                                                \
__NL__         __GLS_DI__(func) (PPG_Action_Callback_Fun)                          \
__NL__                    kaleidoscope::papageno::Papageno::processKeypos,     \
__NL__         __GLS_DI__(user_data) (void*)(uint16_t((ROW) << 8 | (COL)))         \
__NL__      }                                                                  \
__NL__   } 
   
#define GLS_ACTION_INITIALIZE___USER_FUNCTION(UNIQUE_ID, USER_ID, FUNC, USER_DATA)             \
__NL__   {                                                                     \
__NL__      __GLS_DI__(callback)  {                                                \
__NL__         __GLS_DI__(func) (PPG_Action_Callback_Fun)FUNC,                     \
__NL__         __GLS_DI__(user_data) USER_DATA                                     \
__NL__      }                                                                  \
__NL__   } 

// A file that is included in the Glockenspiel-generated
// representation of the Papageno tree just before any C/C++
// stuff is defined. 
// Papageno-Initilization.h contains all the stuff that deals
// with actions and inputs and their mapping to Kaleidoscope data.
//
#define GLS_GLOBAL_INITIALIZATION_INCLUDE "Kaleidoscope/Papageno-Initialization.h"

// Use an artificial event queue size to make sure that there is enough
// space for synthesized loop events.
//
// #define GLS_EVENT_QUEUE_SIZE(S) (2*(S))

/*
glockenspiel_begin

% The following ways to specify inputs are available 
%

% The names of KEYCODE inputs must match exactly the names of 
% Kaleidoscope keycodes.

% A complex keycode. Assign a unique name and pass the keycode specification
% code between dollars (raw code).
%
input: a_complex_keycode_input <COMPLEX_KEYCODE> = $LSHFT(Key_1)$

% A matrix key position based input. Specify the row and colum index as 
% raw C++ code between dollars.
%
input: a_key_at_a_position <KEYPOS> = $1, 2$

% If the same input is supposed to be used under a different name, 
% an alias can be used.
%
alias: some_key Key_A

% Important: Never assign the same name to an input twice.

The following actions are available

% A simple keycode based action. The keycode specified is triggered
% when the action fires. the name of the action must match one of
% Kaleidoscope's predefined keys.
%
action: Key_B <KEYCODE>

% A complex keycode. Assign a unique name and pass the keycode specification
% code between dollars (raw code). The keycode is triggered once the
% action fires.
%
action: a_complex_keycode_action <COMPLEX_KEYCODE> = $RSHFT(Key_2)$

% An action that triggers a key at a given position of the current layer.
%
action: my_keypos_action <KEYPOS> = $1, 3$

% A user function action. Pass a function and an additional pointer to 
% context specific user data (pass 'nullptr' if unused).
%
action: my_func <USER_FUNCTION> = $the_function_name, nullptr$

glockenspiel_end
*/

// For simple application cases with only keypos/keycode input
// and keycode actions, the user is not to be bothered with
// including an additional include file. That's why 
// we include the generated Papageno tree definition file right here.
// 
// Complex use, including the specification of user function actions,
// requires to postpone inclusion of Kaleidoscope-Papageno-Sketch.hpp
// which is caused by defining macro KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS
// before this header is included.
//
#ifndef KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS
extern "C" {
#include "Kaleidoscope-Papageno-Sketch.hpp"
}
#endif
