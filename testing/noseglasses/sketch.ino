// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

// The Kaleidoscope core
#include "Kaleidoscope.h"

#include <kaleidoscope/hid.h>

// Support for papageno features
#define KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS
#include "Kaleidoscope-Papageno.h"

// Support for keys that move the mouse
#include "Kaleidoscope-MouseKeys.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

#include "Kaleidoscope-LED-ActiveModColor.h"

// Support for an "LED off mode"
#include "LED-Off.h"

#include "Kaleidoscope-OneShot.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

enum { MACRO_VERSION_INFO,
       MACRO_ANY
     };

enum { NORMAN, M1, M2, M3 }; // layers

const Key keymaps[][ROWS][COLS] PROGMEM = {

  [NORMAN] = KEYMAP_STACKED
  (___,  ___,   ___, ___, ___,      LCTRL(Key_X),      LCTRL(Key_X),
   OSL(M3),      Key_Q,         Key_W,       Key_D,       Key_F,    Key_K,    LCTRL(Key_C),
   OSL(M2),      Key_A,         Key_S,       Key_E,       Key_T,    Key_G, // home row
   OSM(LeftControl),Key_Z,         Key_X,       Key_C,       Key_V,    Key_B,    LCTRL(Key_V),
   LCTRL(Key_F),          Key_Backspace, OSM(LeftShift),OSM(LeftAlt),
   OSM(LeftAlt),

   ___,          ___,           ___,         ___, ___,     ___, ___,
   ___,          Key_J,         Key_U,       Key_R,       Key_L,    Key_Semicolon, Key_Escape,
                 Key_Y,         Key_N,       Key_I,       Key_O,    Key_H,    OSL(M2),
   LCTRL(Key_S),          Key_P,         Key_M,       Key_Comma,   Key_Period,Key_Slash,OSM(LeftControl),
   OSM(LeftAlt),          OSL(M1),      Key_Space,    ___,
   OSM(LeftAlt)),
   
   [M1] = KEYMAP_STACKED
   (___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         LSHIFT(Key_2),        LSHIFT(Key_Minus), Key_LeftBracket, Key_RightBracket, LSHIFT(Key_6), ___,
    ___,         Key_Backslash, Key_Backtick,   LSHIFT(Key_LeftBracket), LSHIFT(Key_RightBracket), LSHIFT(Key_8),
    ___,         LSHIFT(Key_3),      LSHIFT(Key_Backtick),   Key_Pipe,    LSHIFT(Key_4), RALT(Key_5), ___,
    ___,         ___,           ___,         ___,        
    ___,
    
    ___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         LSHIFT(Key_1), LSHIFT(Key_Comma),  LSHIFT(Key_Period),    Key_Equals, LSHIFT(Key_7), ___,
                 LSHIFT(Key_Slash), LSHIFT(Key_9), LSHIFT(Key_0), Key_Minus, LSHIFT(Key_Semicolon), ___,
    ___,         LSHIFT(Key_Equals),      LSHIFT(Key_5), LSHIFT(Key_Quote), Key_Quote, Key_Semicolon, ___,
    ___,         ___,           ___,         ___,
    ___),
    
    
   [M2] = KEYMAP_STACKED
   (___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         Key_PageUp,    Key_Backspace, Key_UpArrow,    Key_Delete,  Key_PageDown, ___,
    ___,         Key_Home,      Key_LeftArrow,    Key_DownArrow,    Key_RightArrow, Key_End,
    ___,         XXX,       Key_Tab,     XXX,     Key_Enter, XXX, ___,
    ___,         ___,           ___,         ___,
    ___,
    
    ___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         XXX,       Key_7,       Key_8,       Key_9,    XXX,  ___,
                 XXX,       Key_4,       Key_5,       Key_6,    Key_Period,___,
    ___,         Key_0,         Key_1,       Key_2,       Key_3,    Key_Comma,___,
    ___,         ___,           ___,         ___,
    ___),
    
   [M3] = KEYMAP_STACKED
   (___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         ___,           ___,         ___,         ___,      ___,    
    ___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         ___,           ___,         ___,
    ___,
    
    ___,         ___,           ___,         ___,         ___,      ___,      ___,
    ___,         XXX,       Key_F7,      Key_F8,      Key_F9,   Key_F12,  ___,
                 XXX,        Key_F4,      Key_F5,      Key_F6,   Key_F11,  ___,
    ___,         XXX,        Key_F1,      Key_F2,      Key_F3,   Key_F10,  ___,   
    ___,         ___,           ___,         ___,
    ___) 

};

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

  case MACRO_VERSION_INFO:
    versionInfoMacro(keyState);
    break;
   }
  return MACRO_NONE;
}

/** The 'setup' function is one of the two standard Arduino sketch functions.
  * It's called when your keyboard first powers up. This is where you set up
  * Kaleidoscope and any plugins.
  */

void setup() {
   
   // For usb serial debugging
   //
//    Serial.begin(9600);
//    Serial.println("setup");
   
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

    
// #if 0
  // Next, tell Kaleidoscope which plugins you want to use.
  // The order can be important. For example, LED effects are
  // added in the order they're listed here.
  Kaleidoscope.use(
    
    &Papageno,
 
    &OneShot,
    
    // LEDControl provides support for other LED modes
    &LEDControl,
        
    // We start with the LED effect that turns off all the LEDs.
//     &LEDOff,
    
    &ActiveModColorEffect,
    
    // The boot greeting effect pulses the LED button for 10 seconds after the keyboard is first connected
    &BootGreetingEffect
  ); 
}

void loop() {
  Papageno.loop();
}

/*
glockenspiel_begin

%###############################################################################
% Papageno configuration
%###############################################################################

default: event_timeout =  $ 200 $ 
   
%###############################################################################
% Inputs
%###############################################################################

% Thumb keys are numbered left to right
%
input: LeftThumb1 <KEYPOS> =  $ 0,  7 $ 
input: LeftThumb2 <KEYPOS> =  $ 1,  7 $ 
input: LeftThumb3 <KEYPOS> =  $ 2,  7 $ 
input: LeftThumb4 <KEYPOS> =  $ 3,  7 $ 

input: RightThumb1 <KEYPOS> =  $ 3,  8 $ 
input: RightThumb2 <KEYPOS> =  $ 2,  8 $ 
input: RightThumb3 <KEYPOS> =  $ 1,  8 $ 
input: RightThumb4 <KEYPOS> =  $ 0,  8 $ 

input: Special1 <KEYPOS> =  $ 2,  9 $ 
input: Special2 <KEYPOS> =  $ 0,  9 $ 
input: Special3 <KEYPOS> =  $ 0,  6 $ 
input: Special4 <KEYPOS> =  $ 1,  6 $ 
input: Special5 <KEYPOS> =  $ 2,  6 $ 
input: Special6 <KEYPOS> =  $ 1,  9 $ 

% Key alias for keys on Norman layout
%
input: NG_Key_E <KEYPOS> = $ 2,  3 $
input: NG_Key_O <KEYPOS> = $ 2, 13 $
input: NG_Key_I <KEYPOS> = $ 2, 12 $
input: NG_Key_S <KEYPOS> = $ 2,  2 $

input: NG_Prog_Key <KEYPOS> = $ 0,  0 $

%alias: a = RightThumb1
%alias: b = RightThumb2
%alias: c = RightThumb3

%###############################################################################
% Actions
%###############################################################################

action: aShiftTab <COMPLEX_KEYCODE> =  $ LSHIFT(Key_Tab) $ 
action: leftCTRL_R <COMPLEX_KEYCODE> =  $ LCTRL(Key_R) $ 
action: shiftCtrlC <COMPLEX_KEYCODE> =  $ LCTRL(LSHIFT(Key_C)) $ 

action: doubleTab <USER_FUNCTION> =  $ doubleTabCB, NULL $ 
action: repeatLastCommand <USER_FUNCTION> =  $ repeatLastCommandCB, NULL $ 
action: ordinarySearch <USER_FUNCTION> =  $ ordinarySearchCB, NULL $ 
action: fileSearch <USER_FUNCTION> =  $ fileSearchCB, NULL $ 

action: umlaut_A <USER_FUNCTION> =  $ umlautCB, (void*)Key_A.raw $ 
action: umlaut_O <USER_FUNCTION> =  $ umlautCB, (void*)Key_O.raw $ 
action: umlaut_U <USER_FUNCTION> =  $ umlautCB, (void*)Key_U.raw $ 
action: umlaut_S <COMPLEX_KEYCODE> =  $ RALT(Key_S) $ 

action: toggleLEDEffect <COMPLEX_KEYCODE> =  $ Key_LEDEffectNext $ 

action: reboot <USER_FUNCTION> =  $ rebootCB, NULL $ 

%###############################################################################
% Patterns
%###############################################################################

% A cluster that causes enter (key order arbitrary)
%
{LeftThumb3, RightThumb2} : Key_Enter

% Allow to trigger enter with one hand if necessary
%
|LeftThumb4|*2 : Key_Enter
|LeftThumb1|*2 : Key_Enter

% Double tap on the left inner thumb key triggers a user function
%
|LeftThumb3|*2 : doubleTab

% A note line with two thumb keys triggers tab
%
|LeftThumb3| -> |RightThumb3| : Key_Tab

% Double tap on right inner thumb key
%
|RightThumb2|*2 : aShiftTab

% Note line 
%
|RightThumb2| -> |LeftThumb2| : Key_Delete

% Tap dance
%
|Special1|*2 : repeatLastCommand

|Special3|*3 : ordinarySearch@2, fileSearch@3

|Special4|*2 : leftCTRL_R

|Special5|*2 : Key_F3

|Special6|*2 : shiftCtrlC

|NG_Prog_Key|*5 : toggleLEDEffect@2, reboot@5

% Assign german umlauts as tripple taps to
% suitable and non-colliding (digraphs!) keys of the home row
%
% Occurence probabilities of umlauts in written german:
%
% a-umlaut  0,54%
% o-umlaut  0,30%
% u-umlaut  0,65%
% s-umlaut  0,37%
%
% These are used together with the EURKEY keyboard layout
% that is available on Linux and Windows
%
% TODO: Disable the following on layers higher than 0
%
|NG_Key_E|*3 : umlaut_A
|NG_Key_O|*3 : umlaut_O
|NG_Key_I|*3 : umlaut_U
|NG_Key_S|*3 : umlaut_S

%phrase: my_phrase = |LeftThumb3| -> |RightThumb3|
%|Special1| -> |Special3| -> #my_phrase*5 : shiftCtrlC@5 -> |RightThumb3| : shiftCtrlC

% Sequence strings
%"abaca" : shiftCtrlC
%"abaaa" : shiftCtrlC
%"abbaaa" : shiftCtrlC


glockenspiel_end
*/

inline
void pressKey(const Key &k) {
   handleKeyswitchEvent(k, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();
}

inline
void releaseKey(const Key &k) {
   handleKeyswitchEvent(k, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();
}

inline 
void tapKey(const Key &k) {
   pressKey(k);
   releaseKey(k);
}

#define PPG_CALLBACK_NO_REPEAT \
   if(activation_flags & PPG_Action_Activation_Flags_Repeated) { return; }
   
#define PPG_CALLBACK_ONLY_ACTIVATION \
   if(!(activation_flags & PPG_Action_Activation_Flags_Active)) { return; }
   
// User callback the emulates double tab for
// shell auto completion
//
void doubleTabCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION
   
   tapKey(Key_Tab);
   tapKey(Key_Tab);
}

// User callback that repeats the most recent shell
// command
//
void repeatLastCommandCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION
   
   tapKey(Key_UpArrow);
   tapKey(Key_Enter);
}

// Issues a search command that can be used with
// any editor that is configured in a way that F1
// opens the search entry with the string that the cursor
// currently rests on.
//
void ordinarySearchCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION
   
   tapKey(LCTRL(Key_F));
   tapKey(Key_Enter);
}

// Similar the search callback above, but for a search
// in multiple files. This works with editors
// that have been customized to feature Shift+F1
// as command to open the search-in-files menu.
//
void fileSearchCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION
   
   tapKey(LSHIFT(Key_F1));
   tapKey(Key_Enter);
}

void umlautCB(PPG_Count activation_flags, void *user_data)
{
//    PPG_CALLBACK_NO_REPEAT
   
   Key k;
   k.raw = (uint16_t)user_data;
   
   PPG_LOG("umlautCB\n")
   
   // Note, as we might be dealing with one-shot shift, we cannot
   // inject the keys (as one-shot does not deal with injected stuff)
   //
   if(activation_flags & PPG_Action_Activation_Flags_Active) {
      handleKeyswitchEvent(RALT(k), UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED/* | INJECTED*/);
   }
   else {    
      PPG_LOG("releasing umlaut\n") 
      handleKeyswitchEvent(RALT(k), UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED/* | INJECTED*/);
   }
}

void rebootCB(PPG_Count activation_flags, void *user_data)
{
   #ifndef ARDUINO_VIRTUAL
   KeyboardHardware.rebootBootloader();
   #endif 
}

// TODO: Add a mode of operation where Papageno only tracks events
//       but passes them on instead of swallowing them.

extern "C" {
#include "Kaleidoscope-Papageno-Sketch.hpp"
}
