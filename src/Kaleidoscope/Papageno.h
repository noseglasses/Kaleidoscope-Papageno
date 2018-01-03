/* -*- mode: c++ -*-
 * Kaleidoscope-Papageno -- Papageno features for Kaleidoscope
 * Copyright (C) 2017 Florian Fleissner
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

#ifdef PAPAGENO_COMPRESSION_ENABLED
#define PPG_CALLBACK__(...) { return __VA_ARGS__; }
#else
#define PPG_CALLBACK__(...) ;
#endif

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

      inline
      static void setTimeoutMillis(uint16_t timeout)
      {
         ppg_global_set_timeout((PPG_Time)timeout);
      }
      
   private:

      static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
      static void loopHook(bool is_post_clear);
};

} // namespace papageno
} // namespace kaleidoscope

// Override this function to setup your own Papageno patterns
//
void papageno_setup();

extern kaleidoscope::papageno::Papageno Papageno;

enum { PPG_KLS_Not_An_Input = (PPG_Input_Id)-1 };

// The following extern entities are defined through preprocessor
// macros below, e.g. from papageno_setup
//
extern PPG_KLS_Keypos ppg_kls_keypos_lookup[];
extern Key ppg_kls_keycode_lookup[];

extern PPG_Input_Id inputIdFromKeypos(byte row, byte col);
extern PPG_Input_Id inputIdFromKeycode(Key keycode);

extern int16_t highestKeyposInputId();

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

#define PPG_KLS_STRINGIZE(S) #S

#ifdef PAPAGENO_COMPRESSION_ENABLED

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOL(S)                              \
         PPG_COMPRESSION_REGISTER_SYMBOL(ccontext, S)
   
   #define PPG_KLS_INIT_COMPRESSION                                            \
__NN__                                                                         \
__NL__   PPG_Compression_Context ccontext =                                    \
__NL__              ppg_compression_init();                                    \
__NL__                                                                         \
__NL__   PPG_KLS_COMPRESSION_REGISTER_SYMBOL(                                  \
__NN__              kaleidoscope::papageno::Papageno::processKeycode)          \
__NL__   PPG_KLS_COMPRESSION_REGISTER_SYMBOL(                                  \
                    kaleidoscope::papageno::Papageno::processEventCallback)

   #define PPG_KLS_COMPRESSION_RUN                                             \
__NL__   ppg_compression_run(ccontext, "kaleidoscope");                        \
__NL__   ppg_compression_finalize(ccontext);

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOLS(REGISTRATION_MFUNC)            \
      REGISTRATION_MFUNC(PPG_KLS_COMPRESSION_REGISTER_SYMBOL)
      
   #define PPG_KLS_COMPRESSION_CALLBACK_STUB(NAME)                             \
      void NAME() {}
      
   #define PPG_KLS_COMPRESSION_PREPARE_SYMBOLS(REGISTRATION_MFUNC)             \
      REGISTRATION_MFUNC(PPG_KLS_COMPRESSION_CALLBACK_STUB)

#else

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOL(S)
   #define PPG_KLS_INIT_COMPRESSION
   #define PPG_KLS_COMPRESSION_RUN
   
   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOLS(REGISTRATION_MFUNC)
   #define PPG_KLS_COMPRESSION_PREPARE_SYMBOLS(REGISTRATION_MFUNC)
#endif

#define PPG_KLS_COMPILE                                                        \
__NN__                                                                         \
__NN__   ppg_global_compile();                                                 \
__NL__   PPG_KLS_COMPRESSION_RUN

#define PPG_KLS_KEYPOS_HEX(COL_HEX, ROW_HEX, S___)                             \
   S___(COL_HEX, ROW_HEX)

#define PPG_KLS_ACTION_KEYCODE(KK)                                             \
__NL__   (PPG_Action) {                                                        \
__NL__      .callback = (PPG_Action_Callback) {                                \
__NL__         .func = (PPG_Action_Callback_Fun)                               \
__NL__                    kaleidoscope::papageno::Papageno::processKeycode,    \
__NL__         .user_data = reinterpret_cast<void*>(KK.raw)                    \
__NL__      }                                                                  \
__NL__   }

#define PPG_KLS_INIT                                                           \
__NL__                                                                         \
__NL__   ppg_global_init();                                                    \
__NL__                                                                         \
__NL__   Papageno.init();                                                      \
__NL__                                                                         \
__NL__   PPG_KLS_INIT_COMPRESSION
   
#define PPG_KLS_KEYS(...) PPG_INPUTS(__VA_ARGS__)

#define PPG_KLS_TRICAT(S1, S2, S3) S1##S2##S3

#define PPG_KLS_KEYPOS_ENUM(KEYPOS_NAME)                                       \
   PPG_KLS_TRICAT(PPG_, KEYPOS_NAME, _Keypos_Name)

#define PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL(COL_HEX, ROW_HEX)                 \
   256*0x##ROW_HEX + 0x##COL_HEX

#define PPG_KLS_DEFINE_KEYPOS_ENUM(KEYPOS_NAME)                                \
__NL__   enum { PPG_KLS_KEYPOS_ENUM(KEYPOS_NAME)                               \
__NL__               = __COUNTER__ - PPG_Keypos_Input_Offset - 1 };

#define PPG_DEFINE_KEYPOS_INPUT_ENUMS                                          \
__NL__                                                                         \
__NL__   enum { PPG_Keypos_Input_Offset = __COUNTER__ };                       \
__NL__                                                                         \
__NL__   PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_DEFINE_KEYPOS_ENUM)            \
__NL__                                                                         \
__NL__   /* Careful, PPG_Highest_Keypos_Input may be negative in case          \
__NL__      that no keypos inputs are defined                                  \
__NL__   */                                                                    \
__NL__   enum { PPG_Highest_Keypos_Input                                       \
__NL__              = (int16_t)(__COUNTER__) - PPG_Keypos_Input_Offset - 2 };  \
__NL__                                                                         \
__NL__   int16_t highestKeyposInputId() {                                      \
__NL__      return PPG_Highest_Keypos_Input;                                   \
__NL__   }

#define PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS)                                    \
   PPG_##KEYCODE_ALIAS##_Keycode_Name

#define PPG_KLS_DEFINE_KEYCODE_ENUM(KEYCODE_ALIAS)                             \
__NL__   enum { PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS)                            \
__NL__               = PPG_Highest_Keypos_Input + __COUNTER__ - PPG_Keycode_Input_Offset };

#define PPG_DEFINE_KEYCODE_INPUT_ENUMS                                         \
__NL__                                                                         \
__NL__   enum { PPG_Keycode_Input_Offset = __COUNTER__ };                      \
__NL__                                                                         \
__NL__   PPG_KLS_KEYCODE_INPUTS(PPG_KLS_DEFINE_KEYCODE_ENUM)                   \
__NL__                                                                         \
__NL__   /* Careful, PPG_Highest_Keycode_Input may be negative in case that    \
__NL__      no keypos and keycode inputs are defined                           \
__NL__    */                                                                   \
__NL__   enum { PPG_Highest_Keycode_Input = PPG_Highest_Keypos_Input           \
__NL__                    - __COUNTER__ - PPG_Keypos_Input_Offset - 1 };

#define PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL(COL_HEX, ROW_HEX)                 \
   256*0x##ROW_HEX + 0x##COL_HEX

#define PPG_KLS_KEYPOS_CASE_LABEL(KEYPOS_ALIAS)                                \
__NL__   case KEYPOS_ALIAS(PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL):              \
__NL__      return PPG_KLS_KEYPOS_ENUM(KEYPOS_ALIAS);                          \
__NL__      break;
   
#define PPG_INIT_INPUT_ID_FROM_KEYPOS_FUNCTION                                 \
__NL__                                                                         \
__NL__   PPG_Input_Id inputIdFromKeypos(byte row, byte col)                    \
__NL__   {                                                                     \
__NL__      uint16_t id = 256*row + col;                                       \
__NL__                                                                         \
__NL__      switch(id) {                                                       \
__NL__                                                                         \
__NL__         PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_KEYPOS_CASE_LABEL)       \
__NL__      }                                                                  \
__NL__                                                                         \
__NL__      return PPG_KLS_Not_An_Input;                                       \
__NL__   } 

#define PPG_KLS_KEYCODE_CASE_LABEL(KEYCODE_ALIAS)                              \
__NL__   case (KEYCODE_ALIAS):                                                 \
__NL__      return PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS);                        \
__NL__      break;

#define PPG_INIT_INPUT_ID_FROM_KEYCODE_FUNCTION                                \
__NL__                                                                         \
__NL__   PPG_Input_Id inputIdFromKeycode(Key keycode)                          \
__NL__   {                                                                     \
__NL__      switch(keycode.raw) {                                              \
__NL__                                                                         \
__NL__         PPG_KLS_KEYCODE_INPUTS(PPG_KLS_KEYCODE_CASE_LABEL)              \
__NL__      }                                                                  \
__NL__                                                                         \
__NL__      return PPG_KLS_Not_An_Input;                                       \
__NL__   }
  
#define PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(COL_HEX, ROW_HEX)            \
   { .row = 0x##ROW_HEX, .col = 0x##COL_HEX }
   
#define PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY(COL_HEX, ROW_HEX)                \
   PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(COL_HEX, ROW_HEX),
   
#define PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY(KEYPOS_ALIAS)                           \
   KEYPOS_ALIAS(PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY)
   
#define PPG_KLS_INIT_KEYPOS_LOOKUP                                             \
__NL__                                                                         \
__NL__   PPG_KLS_Keypos ppg_kls_keypos_lookup[] = {                            \
__NL__                                                                         \
__NL__      PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY)     \
__NL__                                                                         \
__NL__      PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(FF, FF)                  \
__NL__   };

#define PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX(KEYCODE) KEYCODE

#define PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY(KEYCODE)                        \
   PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX(KEYCODE),
   
#define PPG_KLS_INIT_KEYCODE_LOOKUP                                            \
__NL__                                                                         \
__NL__   Key ppg_kls_keycode_lookup[] = {                                      \
__NL__                                                                         \
__NL__      PPG_KLS_KEYCODE_INPUTS(PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY)     \
__NL__                                                                         \
__NL__      PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX(                        \
__NL__                    (Key){ .raw = (uint16_t)-1})                         \
__NL__   };
   
#define PPG_KLS_INPUT_FROM_KEYPOS_CALL(COL_HEX, ROW_HEX)                       \
   inputIdFromKeypos(0x##ROW_HEX, 0x##COL_HEX)
   
#define PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(KEYPOS_ALIAS)                          \
   PPG_KLS_KEYPOS_ENUM(KEYPOS_ALIAS)
   
#define PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(KEYCODE_ALIAS)                        \
   PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS)
   
#define PPG_KLS_ADD_ONE(KEYPOS_ALIAS)                                          \
   + 1
   
#define PPG_KLS_STORE_N_INPUTS                                                 \
__NL__   enum { PPG_KLS_N_Inputs = 0                                           \
__NL__      PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_ADD_ONE)                    \
__NL__      PPG_KLS_KEYCODE_INPUTS(PPG_KLS_ADD_ONE)                            \
__NL__   };

#define PPG_KLS_INIT_DATA_STRUCTURES                                           \
__NL__                                                                         \
__NL__   PPG_DEFINE_KEYPOS_INPUT_ENUMS                                         \
__NL__                                                                         \
__NL__   PPG_DEFINE_KEYCODE_INPUT_ENUMS                                        \
__NL__                                                                         \
__NL__   PPG_INIT_INPUT_ID_FROM_KEYPOS_FUNCTION                                \
__NL__                                                                         \
__NL__   PPG_INIT_INPUT_ID_FROM_KEYCODE_FUNCTION                               \
__NL__                                                                         \
__NL__   PPG_KLS_INIT_KEYPOS_LOOKUP                                            \
__NL__                                                                         \
__NL__   PPG_KLS_INIT_KEYCODE_LOOKUP                                           \
__NL__                                                                         \
__NL__   PPG_KLS_STORE_N_INPUTS

#define PPG_KLS_CONNECT                                                        \
__NL__                                                                         \
__NL__   void matrix_init_user() {                                             \
__NL__      init_papageno();                                                   \
__NL__   }                                                                     \
__NL__                                                                         \
__NL__   void matrix_scan_user() {                                             \
__NL__      matrix_scan();                                                     \
__NL__   };                                                                    \
__NL__                                                                         \
__NL__   void action_exec_user(keyevent_t event) {                             \
__NL__      process_event(event);                                              \
__NL__   }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Some macros to simplify definition of patterns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "boost/preprocessor/tuple/to_list.hpp"
#include "boost/preprocessor/list/for_each.hpp"

#define PPG_KLS_NOTE_CREATE_STANDARD_KEYPOS(r, data, elem)                     \
__NL__   ppg_note_create_standard(PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(elem)),

#define PPG_KLS_NOTE_CREATE_STANDARD_KEYCODE(r, data, elem)                    \
__NL__   ppg_note_create_standard(PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(elem)),
   
#define PPG_KLS_ACTION_GENERATOR_KEYCODE(KEYCODE)                              \
__NL__   PPG_KLS_ACTION_KEYCODE(                                               \
__NL__         KEYCODE                                                         \
__NL__   )

#define PPG_KLS_PATTERN__(LAYER, ACTION, NOTE_GENERATOR, ACTION_GENERATOR, ...)\
__NL__   ppg_token_set_action(                                                 \
__NL__      ppg_pattern(                                                       \
__NL__         (LAYER),                                                        \
__NL__         PPG_TOKENS(                                                     \
__NL__                                                                         \
__NL__            /* This generates a list of notes                            \
__NL__             */                                                          \
__NL__            BOOST_PP_LIST_FOR_EACH(NOTE_GENERATOR, _,                    \
__NL__                        BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__)))           \
__NL__         )                                                               \
__NL__      ),                                                                 \
__NL__      ACTION_GENERATOR(ACTION)                                           \
__NL__   )
   
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Single note lines
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_NOTE_LINE_ACTION_KEYCODE(LAYER, ACTION, ...)            \
                                                                               \
   PPG_KLS_PATTERN__(LAYER,                                                    \
                     ACTION,                                                   \
                     PPG_KLS_NOTE_CREATE_STANDARD_KEYPOS,                      \
                     PPG_KLS_ACTION_GENERATOR_KEYCODE,                         \
                     __VA_ARGS__                                               \
   )
   
#define PPG_KLS_KEYCODE_NOTE_LINE_ACTION_KEYCODE(LAYER, ACTION, ...)           \
                                                                               \
   PPG_KLS_PATTERN__(LAYER,                                                    \
                     ACTION,                                                   \
                     PPG_KLS_NOTE_CREATE_STANDARD_KEYCODE,                     \
                     PPG_KLS_ACTION_GENERATOR_KEYCODE,                         \
                     __VA_ARGS__                                               \
   )
   
#define PPG_KLS_KEYPOS_AGGREGATE_MEMBER__(r, data, elem)                       \
__NL__   PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(elem),

#define PPG_KLS_KEYCODE_AGGREGATE_MEMBER__(r, data, elem)                      \
__NL__   PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(elem),
   
#define PPG_KLS_AGGREGATE_TOKEN__(TYPE, NOTE_GENERATOR, ...)                   \
                                                                               \
__NL__    PPG_##TYPE##_CREATE(                                                 \
__NL__      BOOST_PP_LIST_FOR_EACH(NOTE_GENERATOR, _,                          \
__NL__                        BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__)))           \
__NL__    )
         
#define PPG_KLS_KEYPOS_CHORD_TOKEN(...)                                        \
                                                                               \
   PPG_KLS_AGGREGATE_TOKEN__( CHORD,                                           \
                              PPG_KLS_KEYPOS_AGGREGATE_MEMBER__,               \
                              __VA_ARGS__)
                              
#define PPG_KLS_KEYCODE_CHORD_TOKEN(...)                                       \
                                                                               \
   PPG_KLS_AGGREGATE_TOKEN__( CHORD,                                           \
                              PPG_KLS_KEYCODE_AGGREGATE_MEMBER__,              \
                              __VA_ARGS__)

#define PPG_KLS_KEYPOS_CLUSTER_TOKEN(...)                                      \
                                                                               \
   PPG_KLS_AGGREGATE_TOKEN__( CLUSTER, \
                              PPG_KLS_KEYPOS_AGGREGATE_MEMBER__,               \
                              __VA_ARGS__)
                              
#define PPG_KLS_KEYCODE_CLUSTER_TOKEN(...)                                     \
                                                                               \
   PPG_KLS_AGGREGATE_TOKEN__( CLUSTER,                                         \
                              PPG_KLS_KEYCODE_AGGREGATE_MEMBER__,              \
                              __VA_ARGS__)

#define PPG_KLS_KEYPOS_KEYS(...)                                               \
__NL__   PPG_KLS_KEYS(                                                         \
__NL__      BOOST_PP_LIST_FOR_EACH(PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, _,       \
__NL__                                BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__)))   \
__NL__                                                                         \
__NL__      /* The list generated above ends with a comma. To fix this,        \
__NL__       * we add a NULL ptr. Papageno can cope with that.                 \
__NL__       */                                                                \
__NL__      /*NULL*/                                                           \
__NL__   )

#define PPG_KLS_KEYCODE_KEYS(...)                                              \
__NL__   PPG_KLS_KEYS(                                                         \
__NL__      BOOST_PP_LIST_FOR_EACH(PPG_KLS_KEYCODE_AGGREGATE_MEMBER__, _,      \
__NL__                                BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__)))   \
__NL__                                                                         \
__NL__      /* The list generated above ends with a comma. To fix this,        \
__NL__       * we add a NULL ptr. Papageno can cope with that.                 \
__NL__       */                                                                \
__NL__      /*NULL*/                                                           \
__NL__   )

#define PPG_KLS_AGGREGATE__(TYPE, LAYER, ACTION, ACTION_GENERATOR, ...)        \
                                                                               \
__NL__   ppg_##TYPE(                                                           \
__NL__      (LAYER),                                                           \
__NL__      ACTION_GENERATOR(                                                  \
__NL__         ACTION                                                          \
__NL__      ),                                                                 \
__NL__      PPG_KLS_KEYPOS_KEYS(__VA_ARGS__)                                   \
__NL__   )
   
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Chords
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_CHORD_ACTION_KEYCODE(LAYER, ACTION, ...)                \
   PPG_KLS_AGGREGATE__(                                                        \
                        chord,                                                 \
                        LAYER,                                                 \
                        ACTION,                                                \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,                      \
                        __VA_ARGS__                                            \
   )
   
#define PPG_KLS_KEYCODE_CHORD_ACTION_KEYCODE(LAYER, ACTION, ...)               \
   PPG_KLS_AGGREGATE__(                                                        \
                        chord,                                                 \
                        LAYER,                                                 \
                        ACTION,                                                \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,                      \
                        __VA_ARGS__                                            \
   )
  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Clusters
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_CLUSTER_ACTION_KEYCODE(LAYER, ACTION, ...)              \
   PPG_KLS_AGGREGATE__(                                                        \
                        cluster,                                               \
                        LAYER,                                                 \
                        ACTION,                                                \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,                      \
                        __VA_ARGS__                                            \
   )
   
#define PPG_KLS_KEYCODE_CLUSTER_ACTION_KEYCODE(LAYER, ACTION, ...)             \
   PPG_KLS_AGGREGATE__(                                                        \
                        cluster,                                               \
                        LAYER,                                                 \
                        ACTION,                                                \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,                      \
                        __VA_ARGS__                                            \
   )
