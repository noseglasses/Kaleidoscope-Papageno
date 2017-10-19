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

#include <Kaleidoscope-Papageno.h>
#include <kaleidoscope/hid.h>

namespace kaleidoscope {

// --- api ---

   Papageno
      ::Papageno()
{
}

void 
   Papageno
      ::begin() 
{
   this->setupPapageno();
   
   Kaleidoscope.useEventHandlerHook(Papageno::eventHandlerHook);
   Kaleidoscope.useLoopHook(Papageno::loopHook);
}

void
   Papageno
      ::setupPapageno()
{
   // Setup Papageno patterns
   // Call a user callback that performs the setup of a papageno context
   // that is used for pattern matching
   
   // TODO: Make this a weak function
   //
   papageno_setup();
}

// --- hooks ---

Key 
   Papageno
      ::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state)
{
   // Make the current layer available for Papageno
   
   ppg_kls_set_current_layer(how to retreive the layer?);
   
   // Pass key and matrix position to Papageno
   
   TODO: Papageno needs a callback that enables flushing the keys
   
   How can keycodes be passed to Kaleidoscope?
   
   return Key_NoKey;
}

void
   Papageno
      ::loopHook(bool is_post_clear)
{
   // Call the papageno timeout check function
}

}

__attribute__((weak)) 
void papageno_setup();

kaleidoscope::Papageno Papageno;

#define PPG_IMMEDIATE_EVENT_PROCESSING

#ifndef PPG_IMMEDIATE_EVENT_PROCESSING
static PPG_Event ppg_kls_flush_queue[30];
static uint8_t ppg_kls_flush_queue_end;
#endif

inline
static uint8_t ppg_kls_get_keystate(bool pressed)
{
   return ((pressed) ? IS_PRESSED : 0) | INJECTED;
}

static void ppg_kls_enter_keycode(Key keycode, 
                                  bool pressed
                                 )
{
   uint8_t keyState = ((pressed) ? IS_PRESSED : 0) | INJECTED;
   
   // Note: Setting ROWS, COLS will skit keymap lookup
   //
   handleKeyswitchEvent(keycode, ROWS, COLS, keyState);
}

static void ppg_kls_flush_event(PPG_Event *event)
{
   int16_t highest_keypos = ppg_kls_highest_keypos_input();

   uint8_t keyState = ppg_kls_get_keystate(pressed);
      
   // Note: Input-IDs are assigned contiguously
   //
   //       Input ids assigned to keypos-inputs are {0..PPG_Highest_Keypos_Input}
   //       
   //       Input ids assigned to keycode-inputs are {PPG_Highest_Keypos_Input + 1..PPG_Highest_Keycode_Input}

   if(event->input > highest_keypos) {
      
      // Map the input to a range starting from zero to be suitable
      // for lookup in the ppg_kls_keycode_lookup array
      //
      uint16_t keycode = ppg_kls_keycode_lookup[event->input - highest_keypos - 1];
      
      // Note: Setting ROWS, COLS will skip keymap lookup
      //
      handleKeyswitchEvent(keycode, ROWS, COLS, keyState);
   }
   else {
      
      handleKeyswitchEvent(Key_NoKey, 
                           ppg_kls_keypos_lookup[event->input].row,
                           ppg_kls_keypos_lookup[event->input].col,
                           keyState);
   }
}

void ppg_kls_process_event_callback(   
                              PPG_Event *event,
                              void *user_data)
{
   // Ignore events that were considered, i.e. swallowed by Papageno
   //
   if(event->flags & PPG_Event_Considered) {

      return; 
   }

   ppg_kls_flush_queue[ppg_kls_flush_queue_end] = *event;
   ++ppg_kls_flush_queue_end;
}

void ppg_kls_flush_events(void)
{  
   ppg_event_buffer_iterate(
      ppg_kls_process_event_callback,
      NULL
   );
}

void ppg_kls_process_keycode(bool activation, void *user_data) {
   
   Key keycode = (Key)user_data;
      
   uint8_t keyState = ppg_kls_get_keystate(pressed);
   
   // Note: Setting ROWS, COLS will skip keymap lookup
   //
   handleKeyswitchEvent(keycode, ROWS, COLS, keyState);
}

void ppg_kls_process_event(Key mappedKey, byte row, byte col, uint8_t keyState)
{   
   
   ...TODO
   
   
   uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(event.key), event.key);
   
   #define PPG_KLS_INPUT_CHECK_A \
__NL__   ppg_kls_input_id_from_keypos( \
__NL__                        event.key.row, \
__NL__                        event.key.col)

   #define PPG_KLS_INPUT_CHECK_B \
         ppg_kls_input_id_from_keycode(keycode)
         
   // The default behavior is to first check it an 
   // input is defined through the keypos of a key.
   // If not then we check the assigned keycode.
   //
   // By defining PPG_KLS_REVERSE_KEYPOS_TO_KEYCODE_PRECEDENCE
   // this order can be reversed.
   //
   #ifndef PPG_KLS_REVERSE_KEYPOS_TO_KEYCODE_PRECEDENCE
      #define PPG_KLS_INPUT_CHECK_1 PPG_KLS_INPUT_CHECK_A
      #define PPG_KLS_INPUT_CHECK_2 PPG_KLS_INPUT_CHECK_B
   #else // PPG_KLS_REVERSE_KEYPOS_TO_KEYCODE_PRECEDENCE
      #define PPG_KLS_INPUT_CHECK_1 PPG_KLS_INPUT_CHECK_B
      #define PPG_KLS_INPUT_CHECK_2 PPG_KLS_INPUT_CHECK_A
   #endif // PPG_KLS_REVERSE_KEYPOS_TO_KEYCODE_PRECEDENCE
         
   uint8_t input = PPG_KLS_INPUT_CHECK_1;
   
   if(input == PPG_KLS_Not_An_Input) { 
      
      input = PPG_KLS_INPUT_CHECK_2;
      
      if(input == PPG_KLS_Not_An_Input) { 
         
         PPG_LOG("not an input\n");

         // Whenever a key occurs that is not an input,
         // we immediately abort pattern matching
         //
         ppg_global_abort_pattern_matching();
         
         
         
         // Let qmk process the key in a regular way
         //
         action_exec(event);
         
         return;
      }
      else {
         PPG_LOG("input %u, keycode %d\n", input, keycode);
      }
   }
// else {
//    uprintf("input %u, row %u, col %u\n", input, record->event.key.row, 
//                         record->event.key.col);
//   }
   
   #ifdef PPG_KLS_ERGODOX_EZ
   ppg_kls_code_key_considered();
   #endif
   
   PPG_Event p_event = {
      .input = input,
      .time = (PPG_Time)event.time,
      .flags = (event.pressed) 
                     ? PPG_Event_Active : PPG_Event_Flags_Empty
   };
   
   uint8_t cur_layer = biton32(layer_state);
   
   ppg_global_set_layer(cur_layer);
   
   ppg_event_process(&p_event);
}

void ppg_kls_time(PPG_Time *time)
{
   uint16_t time_tmp = timer_read();
   *time = *((PPG_Time*)&time_tmp);
}

void  ppg_kls_time_difference(PPG_Time time1, PPG_Time time2, PPG_Time *delta)
{
   uint16_t *delta_t = (uint16_t *)delta;
   
   *delta_t = (uint16_t)time2 - (uint16_t)time1; 
}

int8_t ppg_kls_time_comparison(
                        PPG_Time time1,
                        PPG_Time time2)
{
   if((uint16_t)time1 > (uint16_t)time2) {
      return 1;
   }
   else if((uint16_t)time1 == (uint16_t)time2) {
      return 0;
   }
    
   return -1;
}

void ppg_kls_signal_callback(PPG_Signal_Id signal_id, void *user_data)
{
//    uprintf("signal %u\n", signal_id  );
   
   switch(signal_id) {
      case PPG_On_Abort:
         ppg_kls_flush_events();
         break;
      case PPG_On_Timeout:
         ppg_kls_flush_events();
         break;
      case PPG_On_Match_Failed:
         break;      
      case PPG_On_Flush_Events:
         ppg_kls_flush_events();
         break;
      default:
         return;
   }
   #ifdef PPG_KLS_ERGODOX_EZ
   ppg_kls_code_key_blocked();
   #endif
}

// void ppg_kls_set_timeout_ms(uint16_t timeout)
// {
//    printf("ppg_kls_set_timeout_ms: %d\n", (int)timeout);
//    ppg_global_set_timeout((PPG_Time)timeout);
// }

void ppg_kls_loop_hook(void)
{
   ppg_timeout_check();
}

