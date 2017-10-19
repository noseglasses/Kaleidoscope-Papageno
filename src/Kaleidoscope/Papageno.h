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

namespace kaleidoscope {
class Papageno : public KaleidoscopePlugin {
 public:

  Papageno(void);

  void begin(void) final;

 private:

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void loopHook(bool is_post_clear);
};

}

// Override this function to setup Papageno patterns
//
void papageno_setup();

extern kaleidoscope::Papageno Papageno;

/**
 * This is an interface to the Papageno library. It allows for using 
 * Papageno magic melodies as part of the qmk firmware.
 * 
 * As Papageno's definition of what is an input is very arbitrary, it is possible to
 * use keyboard matix coordinates as well as qmk keycodes to define keys (=inputs in Papageno jargon).
 * 
 * Key press and release actions are passed to Papageno.
 * Use the macro PPG_KLS_INPUT_FROM_KEYPOS_ALIAS to specify a physical key with respect to its
 * hexadecimal row and column ids or the macro PPG_KLS_KEYCODE_KEY to specify a qmk keycode
 * as key identifier.
 * 
 * Please note that layer switching aborts
 * matching of the current pattern and flushes all keypress events that queued up.
*/

#include "papageno.h"

#ifdef PAPAGENO_COMPRESSION_ENABLED
#define PPG_CALLBACK__(...) { return __VA_ARGS__; }
#else
#define PPG_CALLBACK__(...) ;
#endif

typedef struct {
   byte row;
   byte col;
} PPG_KLS_Keypos;

// The following extern entities are defined through preprocessor
// macros in the keymap
//
extern PPG_KLS_Keypos ppg_kls_keypos_lookup[];
extern uint16_t ppg_kls_keycode_lookup[];


extern PPG_Input_Id ppg_kls_input_id_from_keypos(byte row, byte col);
extern PPG_Input_Id ppg_kls_input_id_from_keycode(Key keycode);

extern int16_t ppg_kls_highest_keypos_input(void);

void ppg_kls_process_event_callback(   
                        PPG_Event *key_event,
                        void *user_data) PPG_CALLBACK__()

void ppg_kls_signal_callback(PPG_Signal_Id signal_id, void *user_data) PPG_CALLBACK__()

void ppg_kls_flush_key_events(void);

void ppg_kls_process_keycode(bool activation, void *user_data) PPG_CALLBACK__()

void ppg_kls_process_event(Key mapped_key, byte row, byte col, uint8_t key_state);

void ppg_kls_time(         PPG_Time *time) PPG_CALLBACK__()

void  ppg_kls_time_difference(
                        PPG_Time time1,
                        PPG_Time time2,
                        PPG_Time *delta) PPG_CALLBACK__()

int8_t ppg_kls_time_comparison(
                        PPG_Time time1,
                        PPG_Time time2) PPG_CALLBACK__(0)

inline
void ppg_kls_set_timeout_ms(uint16_t timeout)
{
   ppg_global_set_timeout((PPG_Time)timeout);
}

void ppg_kls_loop_hook(void);

/* Call this to flush key events that
 * were encountered by papageno
 */
void ppg_kls_flush_key_events(void);

enum { PPG_KLS_Not_An_Input = (PPG_Input_Id)-1 };

// Note: Preprocessor macro functions can be 
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

#define PPG_KLS_STRINGIZE(S) #S

#ifdef PAPAGENO_COMPRESSION_ENABLED

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOL(S) \
         PPG_COMPRESSION_REGISTER_SYMBOL(ccontext, S)
   
   #define PPG_KLS_INIT_COMPRESSION \
\
__NL__   PPG_Compression_Context ccontext =  \
__NL__              ppg_compression_init(); \
__NL__   \
__NL__   PPG_KLS_COMPRESSION_REGISTER_SYMBOL(ppg_kls_process_keycode) \
__NL__   PPG_KLS_COMPRESSION_REGISTER_SYMBOL(  \
                                       ppg_kls_process_event_callback)

   #define PPG_KLS_COMPRESSION_RUN \
__NL__   ppg_compression_run(ccontext, "kaleidoscope"); \
__NL__   ppg_compression_finalize(ccontext);

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOLS(REGISTRATION_MFUNC) \
      REGISTRATION_MFUNC(PPG_KLS_COMPRESSION_REGISTER_SYMBOL)
      
   #define PPG_KLS_COMPRESSION_CALLBACK_STUB(NAME) \
      void NAME(void) {}
      
   #define PPG_KLS_COMPRESSION_PREPARE_SYMBOLS(REGISTRATION_MFUNC) \
      REGISTRATION_MFUNC(PPG_KLS_COMPRESSION_CALLBACK_STUB)

#else

   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOL(S)
   #define PPG_KLS_INIT_COMPRESSION
   #define PPG_KLS_COMPRESSION_RUN
   
   #define PPG_KLS_COMPRESSION_REGISTER_SYMBOLS(REGISTRATION_MFUNC)
   #define PPG_KLS_COMPRESSION_PREPARE_SYMBOLS(REGISTRATION_MFUNC)
#endif

#define PPG_KLS_COMPILE \
 \
         ppg_global_compile(); \
__NL__   PPG_KLS_COMPRESSION_RUN

#define PPG_KLS_KEYPOS_HEX(COL_HEX, ROW_HEX, S___) \
   S___(COL_HEX, ROW_HEX)

#define PPG_KLS_ACTION_KEYCODE(KK) \
__NL__   (PPG_Action) { \
__NL__      .callback = (PPG_Action_Callback) { \
__NL__         .func = (PPG_Action_Callback_Fun)ppg_kls_process_keycode,  \
__NL__         .user_data = (void*)(uint16_t)ppg_kls_report_keycode(KK) \
__NL__      } \
__NL__   }

#define PPG_KLS_SETUP_NON_DEFAULT_MANAGERS \
__NL__   \
__NL__   ppg_global_set_default_event_processor( \
__NL__      (PPG_Event_Processor_Fun)ppg_kls_process_event_callback); \
__NL__   \
__NL__   ppg_global_set_signal_callback( \
__NL__      (PPG_Signal_Callback) { \
__NL__            .func = (PPG_Signal_Callback_Fun)ppg_kls_signal_callback, \
__NL__            .user_data = NULL \
__NL__      } \
__NL__   ); \
__NL__   \
__NL__   ppg_global_set_time_manager( \
__NL__      (PPG_Time_Manager) { \
__NL__         .time \
__NL__            = (PPG_Time_Fun)ppg_kls_time, \
__NL__         .time_difference \
__NL__            = (PPG_Time_Difference_Fun)ppg_kls_time_difference, \
__NL__         .compare_times \
__NL__            = (PPG_Time_Comparison_Fun)ppg_kls_time_comparison \
__NL__      } \
__NL__   ); \

#define PPG_KLS_INIT \
__NL__   \
__NL__   ppg_global_init(); \
__NL__   \
__NL__   PPG_KLS_SETUP_NON_DEFAULT_MANAGERS \
__NL__   \
__NL__   PPG_KLS_INIT_COMPRESSION
   
#define PPG_KLS_KEYS(...) PPG_INPUTS(__VA_ARGS__)

#define PPG_KLS_TRICAT(S1, S2, S3) S1##S2##S3

#define PPG_KLS_KEYPOS_ENUM(KEYPOS_NAME) \
   PPG_KLS_TRICAT(PPG_, KEYPOS_NAME, _Keypos_Name)

#define PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL(COL_HEX, ROW_HEX) \
   256*0x##ROW_HEX + 0x##COL_HEX

#define PPG_KLS_DEFINE_KEYPOS_ENUM(KEYPOS_NAME) \
__NL__   enum { PPG_KLS_KEYPOS_ENUM(KEYPOS_NAME) \
__NL__               = __COUNTER__ - PPG_Keypos_Input_Offset - 1 };

#define PPG_DEFINE_KEYPOS_INPUT_ENUMS \
__NL__   \
__NL__   enum { PPG_Keypos_Input_Offset = __COUNTER__ }; \
__NL__   \
__NL__   PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_DEFINE_KEYPOS_ENUM) \
__NL__   \
__NL__   /* Careful, PPG_Highest_Keypos_Input may be negative in case that no keypos \
__NL__      inputs are defined \
__NL__   */ \
__NL__   enum { PPG_Highest_Keypos_Input = (int16_t)(__COUNTER__) - PPG_Keypos_Input_Offset - 2 }; \
__NL__   \
__NL__   int16_t ppg_kls_highest_keypos_input(void) { \
__NL__      return PPG_Highest_Keypos_Input; \
__NL__   }

#define PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS) \
   PPG_##KEYCODE_ALIAS##_Keycode_Name

#define PPG_KLS_DEFINE_KEYCODE_ENUM(KEYCODE_ALIAS) \
__NL__   enum { PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS) \
__NL__               = PPG_Highest_Keypos_Input + __COUNTER__ - PPG_Keycode_Input_Offset };

#define PPG_DEFINE_KEYCODE_INPUT_ENUMS \
__NL__   \
__NL__   enum { PPG_Keycode_Input_Offset = __COUNTER__ }; \
__NL__   \
__NL__   PPG_KLS_KEYCODE_INPUTS(PPG_KLS_DEFINE_KEYCODE_ENUM) \
__NL__   \
__NL__   /* Careful, PPG_Highest_Keycode_Input may be negative in case that no keypos and keycode \
__NL__      inputs are defined \
__NL__    */ \
__NL__   enum { PPG_Highest_Keycode_Input = PPG_Highest_Keypos_Input \
__NL__                    - __COUNTER__ - PPG_Keypos_Input_Offset - 1 };

#define PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL(COL_HEX, ROW_HEX) \
   256*0x##ROW_HEX + 0x##COL_HEX

#define PPG_KLS_KEYPOS_CASE_LABEL(KEYPOS_ALIAS) \
__NL__   case KEYPOS_ALIAS(PPG_KLS_CONVERT_KEYPOS_TO_CASE_LABEL): \
__NL__      return PPG_KLS_KEYPOS_ENUM(KEYPOS_ALIAS); \
__NL__      break;
   
#define PPG_INIT_INPUT_ID_FROM_KEYPOS_FUNCTION \
__NL__   \
__NL__   PPG_Input_Id ppg_kls_input_id_from_keypos(byte row, byte col) \
__NL__   { \
__NL__      uint16_t id = 256*row + col; \
__NL__      \
__NL__      switch(id) { \
__NL__         \
__NL__         PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_KEYPOS_CASE_LABEL) \
__NL__      } \
__NL__      \
__NL__      return PPG_KLS_Not_An_Input; \
__NL__   } 

#define PPG_KLS_KEYCODE_CASE_LABEL(KEYCODE_ALIAS) \
__NL__   case (KEYCODE_ALIAS): \
__NL__      return PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS); \
__NL__      break;

#define PPG_INIT_INPUT_ID_FROM_KEYCODE_FUNCTION \
__NL__   \
__NL__   PPG_Input_Id ppg_kls_input_id_from_keycode(Key keycode) \
__NL__   { \
__NL__      switch(keycode.raw) { \
__NL__         \
__NL__         PPG_KLS_KEYCODE_INPUTS(PPG_KLS_KEYCODE_CASE_LABEL) \
__NL__      } \
__NL__      \
__NL__      return PPG_KLS_Not_An_Input; \
__NL__   }
  
#define PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(COL_HEX, ROW_HEX) \
   { .row = 0x##ROW_HEX, .col = 0x##COL_HEX }
   
#define PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY(COL_HEX, ROW_HEX) \
   PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(COL_HEX, ROW_HEX),
   
#define PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY(KEYPOS_ALIAS) \
   KEYPOS_ALIAS(PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY)
   
#define PPG_KLS_INIT_KEYPOS_LOOKUP \
__NL__   \
__NL__   PPG_KLS_Keypos ppg_kls_keypos_lookup[] = { \
__NL__      \
__NL__      PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_KEYPOS_TO_LOOKUP_ENTRY) \
__NL__      \
__NL__      PPG_KLS_CONVERT_TO_KEYPOS_ARRAY_ENTRY_AUX(FF, FF) \
__NL__   };

#define PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX(KEYCODE) KEYCODE

#define PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY(KEYCODE) \
   PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX(KEYCODE),
   
#define PPG_KLS_INIT_KEYCODE_LOOKUP \
__NL__   \
__NL__   Key ppg_kls_keycode_lookup[] = { \
__NL__      \
__NL__      PPG_KLS_KEYCODE_INPUTS(PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY) \
__NL__      \
__NL__      PPG_KLS_CONVERT_TO_KEYCODE_ARRAY_ENTRY_AUX((uint16_t)-1) \
__NL__   };
   
#define PPG_KLS_INPUT_FROM_KEYPOS_CALL(COL_HEX, ROW_HEX) \
   ppg_kls_input_id_from_keypos(0x##ROW_HEX, 0x##COL_HEX)
   
#define PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(KEYPOS_ALIAS) \
   PPG_KLS_KEYPOS_ENUM(KEYPOS_ALIAS)
   
#define PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(KEYCODE_ALIAS) \
   PPG_KLS_KEYCODE_ENUM(KEYCODE_ALIAS)
   
#define PPG_KLS_ADD_ONE(KEYPOS_ALIAS) \
   + 1
   
#define PPG_KLS_STORE_N_INPUTS \
__NL__   enum { PPG_KLS_N_Inputs = 0 \
__NL__      PPG_KLS_MATRIX_POSITION_INPUTS(PPG_KLS_ADD_ONE) \
__NL__      PPG_KLS_KEYCODE_INPUTS(PPG_KLS_ADD_ONE) \
__NL__   };

#define PPG_KLS_INIT_DATA_STRUCTURES \
__NL__   \
__NL__   PPG_DEFINE_KEYPOS_INPUT_ENUMS \
__NL__   \
__NL__   PPG_DEFINE_KEYCODE_INPUT_ENUMS \
__NL__   \
__NL__   PPG_INIT_INPUT_ID_FROM_KEYPOS_FUNCTION \
__NL__   \
__NL__   PPG_INIT_INPUT_ID_FROM_KEYCODE_FUNCTION \
__NL__   \
__NL__   PPG_KLS_INIT_KEYPOS_LOOKUP \
__NL__   \
__NL__   PPG_KLS_INIT_KEYCODE_LOOKUP \
__NL__   \
__NL__   PPG_KLS_STORE_N_INPUTS

#define PPG_KLS_CONNECT \
__NL__   \
__NL__   void matrix_init_user(void) { \
__NL__      init_papageno(); \
__NL__   } \
__NL__   \
__NL__   void matrix_scan_user(void) { \
__NL__      ppg_kls_matrix_scan(); \
__NL__   }; \
__NL__   \
__NL__   void action_exec_user(keyevent_t event) { \
__NL__      ppg_kls_process_event(event); \
__NL__   }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Some macros to simplify definition of patterns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "boost/preprocessor/tuple/to_list.hpp"
#include "boost/preprocessor/list/for_each.hpp"

#define PPG_KLS_NOTE_CREATE_STANDARD_KEYPOS(r, data, elem) \
__NL__   ppg_note_create_standard(PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(elem)),

#define PPG_KLS_NOTE_CREATE_STANDARD_KEYCODE(r, data, elem) \
__NL__   ppg_note_create_standard(PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(elem)),
   
#define PPG_KLS_ACTION_GENERATOR_KEYCODE(KEYCODE) \
__NL__   PPG_KLS_ACTION_KEYCODE( \
__NL__         KEYCODE \
__NL__   )

#define PPG_KLS_PATTERN__(LAYER, ACTION, NOTE_GENERATOR, ACTION_GENERATOR, ...) \
__NL__   ppg_token_set_action( \
__NL__      ppg_pattern( \
__NL__         (LAYER), \
__NL__         PPG_TOKENS(  \
__NL__            \
__NL__            /* This generates a list of notes \
__NL__             */ \
__NL__            BOOST_PP_LIST_FOR_EACH(NOTE_GENERATOR, _, BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))) \
__NL__         ) \
__NL__      ), \
__NL__      ACTION_GENERATOR(ACTION) \
__NL__   )
   
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Single note lines
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_NOTE_LINE_ACTION_KEYCODE(LAYER, ACTION, ...) \
   \
   PPG_KLS_PATTERN__(LAYER,  \
                     ACTION,  \
                     PPG_KLS_NOTE_CREATE_STANDARD_KEYPOS,  \
                     PPG_KLS_ACTION_GENERATOR_KEYCODE, \
                     __VA_ARGS__ \
   )
   
#define PPG_KLS_KEYCODE_NOTE_LINE_ACTION_KEYCODE(LAYER, ACTION, ...) \
   \
   PPG_KLS_PATTERN__(LAYER,  \
                     ACTION,  \
                     PPG_KLS_NOTE_CREATE_STANDARD_KEYCODE,  \
                     PPG_KLS_ACTION_GENERATOR_KEYCODE, \
                     __VA_ARGS__ \
   )
   
#define PPG_KLS_KEYPOS_AGGREGATE_MEMBER__(r, data, elem) \
__NL__   PPG_KLS_INPUT_FROM_KEYPOS_ALIAS(elem),

#define PPG_KLS_KEYCODE_AGGREGATE_MEMBER__(r, data, elem) \
__NL__   PPG_KLS_INPUT_FROM_KEYCODE_ALIAS(elem),
   
#define PPG_KLS_AGGREGATE_TOKEN__(TYPE, NOTE_GENERATOR, ...) \
   \
__NL__    PPG_##TYPE##_CREATE( \
__NL__      BOOST_PP_LIST_FOR_EACH(NOTE_GENERATOR, _, BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))) \
__NL__    )
         
#define PPG_KLS_KEYPOS_CHORD_TOKEN(...) \
   \
   PPG_KLS_AGGREGATE_TOKEN__( CHORD, \
                              PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, \
                              __VA_ARGS__)
                              
#define PPG_KLS_KEYCODE_CHORD_TOKEN(...) \
   \
   PPG_KLS_AGGREGATE_TOKEN__( CHORD, \
                              PPG_KLS_KEYCODE_AGGREGATE_MEMBER__, \
                              __VA_ARGS__)

#define PPG_KLS_KEYPOS_CLUSTER_TOKEN(...) \
   \
   PPG_KLS_AGGREGATE_TOKEN__( CLUSTER, \
                              PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, \
                              __VA_ARGS__)
                              
#define PPG_KLS_KEYCODE_CLUSTER_TOKEN(...) \
   \
   PPG_KLS_AGGREGATE_TOKEN__( CLUSTER, \
                              PPG_KLS_KEYCODE_AGGREGATE_MEMBER__, \
                              __VA_ARGS__)

#define PPG_KLS_KEYPOS_KEYS(...) \
__NL__   PPG_KLS_KEYS( \
__NL__      BOOST_PP_LIST_FOR_EACH(PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, _, BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))) \
__NL__      \
__NL__      /* The list generated above ends with a comma. To fix this, \
__NL__       * we add a NULL ptr. Papageno can cope with that. \
__NL__       */ \
__NL__      /*NULL*/ \
__NL__   )

#define PPG_KLS_KEYCODE_KEYS(...) \
__NL__   PPG_KLS_KEYS( \
__NL__      BOOST_PP_LIST_FOR_EACH(PPG_KLS_KEYCODE_AGGREGATE_MEMBER__, _, BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))) \
__NL__      \
__NL__      /* The list generated above ends with a comma. To fix this, \
__NL__       * we add a NULL ptr. Papageno can cope with that. \
__NL__       */ \
__NL__      /*NULL*/ \
__NL__   )

#define PPG_KLS_AGGREGATE__(TYPE, LAYER, ACTION, NOTE_GENERATOR, ACTION_GENERATOR, ...) \
\
__NL__   ppg_##TYPE(  \
__NL__      (LAYER), \
__NL__      ACTION_GENERATOR( \
__NL__         ACTION \
__NL__      ), \
__NL__      PPG_KLS_KEYPOS_KEYS(__VA_ARGS__) \
__NL__   )
   
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Chords
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_CHORD_ACTION_KEYCODE(LAYER, ACTION, ...) \
   PPG_KLS_AGGREGATE__( \
                        chord, \
                        LAYER, \
                        ACTION, \
                        PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,   \
                        __VA_ARGS__ \
   )
   
#define PPG_KLS_KEYCODE_CHORD_ACTION_KEYCODE(LAYER, ACTION, ...) \
   PPG_KLS_AGGREGATE__( \
                        chord,  \
                        LAYER,  \
                        ACTION,  \
                        PPG_KLS_KEYCODE_AGGREGATE_MEMBER__,  \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,  \
                        __VA_ARGS__ \
   )
  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Clusters
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define PPG_KLS_KEYPOS_CLUSTER_ACTION_KEYCODE(LAYER, ACTION, ...) \
   PPG_KLS_AGGREGATE__( \
                        cluster, \
                        LAYER, \
                        ACTION, \
                        PPG_KLS_KEYPOS_AGGREGATE_MEMBER__, \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE, \
                        __VA_ARGS__ \
   )
   
#define PPG_KLS_KEYCODE_CLUSTER_ACTION_KEYCODE(LAYER, ACTION, ...) \
   PPG_KLS_AGGREGATE__( \
                        cluster,  \
                        LAYER,  \
                        ACTION,  \
                        PPG_KLS_KEYCODE_AGGREGATE_MEMBER__,  \
                        PPG_KLS_ACTION_GENERATOR_KEYCODE,  \
                        __VA_ARGS__ \
   )
                
#endif
