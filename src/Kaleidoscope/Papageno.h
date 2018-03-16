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
}

typedef struct {
   byte row;
   byte col;
} PPG_KLS_Keypos;

namespace kaleidoscope {
namespace papageno {
   
class Papageno : public KaleidoscopePlugin
{
   public:
      
      void begin() final;
      
      void init();
            
      // Some utility functions required by Papageno's API
      //
      static void processKeycode(bool activation, void *user_data) PPG_CALLBACK__()
      
   private:

      static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
      static void loopHook(bool is_post_clear);
};

} // namespace papageno
} // namespace kaleidoscope

extern kaleidoscope::papageno::Papageno Papageno;

enum { PPG_KLS_Not_An_Input = (PPG_Input_Id)-1 };

// The following extern entities are initialized in Papageno-Initialization.h
//
extern PPG_KLS_Keypos ppg_kls_keypos_lookup[];
extern Key ppg_kls_keycode_lookup[];

extern PPG_Input_Id inputIdFromKeypos(byte row, byte col);
extern PPG_Input_Id inputIdFromKeycode(Key keycode);

extern int16_t highestKeyposInputId();

#define PPG_KLS_TRICAT(S1, S2, S3) S1##S2##S3

#define PPG_KLS_KEYPOS_INPUT(ID)                                               \
   PPG_KLS_TRICAT(PPG_, ID, _Keypos_Name)
   
#define PPG_KLS_KEYCODE_INPUT(ID)                                              \
   PPG_##ID##_Keycode_Name

//##############################################################################
// Definitions for Papageno's Glockenspiel compiler interface
//##############################################################################

// This macro is used by the Glockenspiel compiled code to initialize
// keypos based inputs. It boils down to the name of a constexpr integer that
// references the input.
//
#define GLS_INPUT_INITIALIZE__KEYPOS(ID, ROW, COL) \
   PPG_KLS_KEYPOS_INPUT(ID)

// This macro is used by the Glockenspiel compiled code to initialize
// keycode based inputs. It boils down to the name of a constexpr integer that
// references the input.
//  
#define GLS_INPUT_INITIALIZE__KEYCODE(ID) \
   PPG_KLS_KEYCODE_INPUT(ID)

// This macro is used by the Glockenspiel compiled code to initialize
// keycode based inputs. It boils down to the name of a constexpr integer that
// references the input.
//  
#define GLS_INPUT_INITIALIZE__COMPLEX_KEYCODE(ID, KEYCODE) \
   PPG_KLS_KEYCODE_INPUT(ID)
   
// Keycode actions are compile time constant and can thus already
// assigned when the global static Papageno search tree is initialized.
//
#define GLS_ACTION_INITIALIZE__KEYCODE(ID, KEYCODE)                            \
__NL__   {                                                                     \
__NL__      .callback =  {                                                     \
__NL__         .func = (PPG_Action_Callback_Fun)                               \
__NL__                    kaleidoscope::papageno::Papageno::processKeycode,    \
__NL__         .user_data = reinterpret_cast<void*>(KEYCODE.raw)               \
__NL__      }                                                                  \
__NL__   }                                       
   
// A file that is included in the Glockenspiel-generated
// representation of the Papageno tree just before any C/C++
// stuff is defined. 
// Papageno-Initilization.h contains all the stuff that deals
// with actions and inputs and their mapping to Kaleidoscope data.
//
#define GLS_GLOBAL_INITIALIZATION_INCLUDE "Papageno-Initilization.h"

// The following ways to specify inputs are available 
//
//
// papageno code:
//
// % The names of KEYCODE inputs must match exactly the names of 
// % Kaleidoscope keycodes.
//
// input: Key_A <KEYCODE>
//
// input: a_key_at_a_position <KEYPOS> = $1, 2$
//
// % If the same input is supposed to be used under a different name, 
// % an alias can be used.
// %
// alias: some_key Key_A
//
// % Important: Never assign the same name to an input twice.
