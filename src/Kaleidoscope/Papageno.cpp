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

#include <iostream>

__attribute__((weak)) 
void papageno_setup();

kaleidoscope::papageno::Papageno Papageno;

namespace kaleidoscope {
namespace papageno {

static PPG_Event flushQueue_[30];
static uint8_t flushQueueEnd_;

static bool eventHandlerDisabled = false;

inline
static uint8_t getKeystate(bool pressed)
{
   return ((pressed) ? IS_PRESSED : 0)/* | INJECTED*/;
}

static void flushEvent(PPG_Event *event)
{
   int16_t highest_keypos = highestKeyposInputId();

   uint8_t keyState = kaleidoscope::papageno::getKeystate(
      event->flags & PPG_Event_Active);
      
   // Note: Input-IDs are assigned contiguously
   //
   //       Input ids assigned to keypos-inputs are {0..PPG_Highest_Keypos_Input}
   //       
   //       Input ids assigned to keycode-inputs are {PPG_Highest_Keypos_Input + 1..PPG_Highest_Keycode_Input}

   eventHandlerDisabled = true;
   
   if(event->input > highest_keypos) {
      
      // Map the input to a range starting from zero to be suitable
      // for lookup in the ppg_kls_keycode_lookup array
      //
      Key keycode = ppg_kls_keycode_lookup[event->input - highest_keypos - 1];
      
      PPG_LOG("flushing event by keycode %hu\n", keycode);
              
      // Note: Setting ROWS, COLS will skip keymap lookup
      //
//       handleKeyswitchEvent((Key_){ .raw = keycode}, ROWS, COLS, keyState);
      handleKeyswitchEvent(keycode, ROWS, COLS, keyState);
   }
   else {
      
      PPG_LOG("flushing event by keypos (%hhu, %hhu), keystate = %d\n",
              ppg_kls_keypos_lookup[event->input].row,
              ppg_kls_keypos_lookup[event->input].col,
              (int)keyState
             );
      
      handleKeyswitchEvent(Key_NoKey, 
                           ppg_kls_keypos_lookup[event->input].row,
                           ppg_kls_keypos_lookup[event->input].col,
                           keyState);
   }

   eventHandlerDisabled = false;
   
   // We must be sure to send press and release events
   // in separate keyboard reports.
   //
   kaleidoscope::hid::sendKeyboardReport();
}

static void processEventCallback(   
                              PPG_Event *event,
                              void *user_data)
{
   // Ignore events that were considered, i.e. swallowed by Papageno
   //
   if(event->flags & PPG_Event_Considered) {

      return; 
   }

   papageno::flushQueue_[papageno::flushQueueEnd_] = *event;
   
   ++papageno::flushQueueEnd_;
}

static void flushEvents()
{  
   ppg_event_buffer_iterate(
      processEventCallback,
      NULL
   );
}

static void delayedFlushEvents()
{
   if(papageno::flushQueueEnd_ == 0) { return; }
   
   uint8_t fqe = papageno::flushQueueEnd_;
   papageno::flushQueueEnd_ = 0;
   
   for(uint8_t i = 0; i < fqe; ++i) {

      PPG_Event *event = papageno::flushQueue_ + i;
      
      flushEvent(event);
   }
}

static void time(PPG_Time *time)
{
   uint16_t time_tmp = millis();
   *time = *((PPG_Time*)&time_tmp);
}

static void timeDifference(PPG_Time time1, PPG_Time time2, PPG_Time *delta)
{
   uint16_t *delta_t = (uint16_t *)delta;
   
   *delta_t = (uint16_t)time2 - (uint16_t)time1; 
}

static int8_t timeComparison(
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

static void signalCallback(PPG_Signal_Id signal_id, void *user_data)
{
//    uprintf("signal %u\n", signal_id  );
   
   switch(signal_id) {
      case PPG_On_Abort:
         papageno::flushEvents();
         break;
      case PPG_On_Timeout:
         papageno::flushEvents();
         break;
      case PPG_On_Match_Failed:
         break;      
      case PPG_On_Flush_Events:
         papageno::flushEvents();
         break;
      default:
         return;
   }
   #ifdef PPG_KLS_ERGODOX_EZ
   code_key_blocked();
   #endif
} 

static Key eventHandlerHook(Key keycode, byte row, byte col, uint8_t key_state)
{  
   // When flushing events, this event handler must 
   // be disable as the events are meant to be handled 
   // by the rest of Kaleidoscope.
   //
   if(eventHandlerDisabled) {
      return keycode;
   }
   
   PPG_Count flags = PPG_Event_Flags_Empty;
   bool keyStateChanged = true;
   if (keyToggledOn(key_state)) {
      flags = PPG_Event_Active;
   }
   else if(!keyToggledOff(key_state)) {
      // It the key remains pressed, we just ignore it
      keyStateChanged = false;
   }
   
   #define PPG_KLS_INPUT_CHECK_A \
         inputIdFromKeypos(row, col)

   #define PPG_KLS_INPUT_CHECK_B \
         inputIdFromKeycode(keycode)
         
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
         
//          PPG_LOG("not an input\n");
         
         // Only if another (unrelated) key was pressed we abort.
         //
         if(flags == PPG_Event_Active) {

            // Whenever a key occurs that is not an input,
            // we immediately abort pattern matching
            //
            ppg_global_abort_pattern_matching();
         }
         
         // Let Kaleidoscope process the key in a regular way
         //
         return keycode;
      }
      else {
         PPG_LOG("input %u, keycode %d\n", input, keycode);
      }
   }
// else {
//    uprintf("input %u, row %u, col %u\n", input, record->event.key.row, 
//                         record->event.key.col);
//   }

   if(!keyStateChanged) {
      
      // Just ignore keys that are active but whose state does not change
      //
      return Key_NoKey;
   }
 
   std::cout << "eventHandlerHook: keycode.raw = " << (int)keycode.raw 
      << ", row = " << (int)row << ", col = " << (int)col 
      << ", key_state = " << (int)key_state << std::endl;
      
   PPG_Event p_event = {
      .input = input,
      .time = (PPG_Time)millis(),
      .flags = flags
   };
   
   uint8_t cur_layer = Layer.top();
   
   ppg_global_set_layer(cur_layer);
   
   ppg_event_process(&p_event);
   
   return Key_NoKey;
}

static void loopHook(bool is_post_clear)
{
   if(!is_post_clear) {
      delayedFlushEvents();
      std::cout << "Timeout check" << std::endl;
      ppg_timeout_check();
   }
}

void 
   Papageno
      ::begin() 
{
   // Call the possibly overridden user initialization function
   //
   papageno_setup();
   
   Kaleidoscope.useEventHandlerHook(
         kaleidoscope::papageno::eventHandlerHook);
   
   Kaleidoscope.useLoopHook(
         kaleidoscope::papageno::loopHook);
}

void 
   Papageno
      ::init()
{
   ppg_global_set_default_event_processor(
      (PPG_Event_Processor_Fun)kaleidoscope::papageno::processEventCallback);

   ppg_global_set_signal_callback(
      (PPG_Signal_Callback) {
            .func = (PPG_Signal_Callback_Fun)kaleidoscope::papageno::signalCallback,
            .user_data = NULL
      }
   );

   ppg_global_set_time_manager(
      (PPG_Time_Manager) {
         .time
            = (PPG_Time_Fun)kaleidoscope::papageno::time,
         .time_difference
            = (PPG_Time_Difference_Fun)kaleidoscope::papageno::timeDifference,
         .compare_times
            = (PPG_Time_Comparison_Fun)kaleidoscope::papageno::timeComparison
      }
   );
}

void  
   Papageno
      ::processKeycode(bool activation, void *user_data)
{   
   Key keycode 
      = (Key){ .raw 
            = reinterpret_cast<uint16_t>(user_data)};
      
   uint8_t keyState = kaleidoscope::papageno::getKeystate(activation);
   
   // Note: Setting ROWS, COLS will skip keymap lookup
   //
   handleKeyswitchEvent(keycode, ROWS, COLS, keyState);
}

} // end namespace papageno
} // end namepace kaleidoscope
