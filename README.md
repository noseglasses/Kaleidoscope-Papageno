![status][st:experimental] [![Build Status][travis:image]][travis:status]

[travis:image]: https://travis-ci.org/noseglasses/Kaleidoscope-Papageno.svg?branch=master
[travis:status]: https://travis-ci.org/noseglasses/Kaleidoscope-Papageno

[st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
[st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
[st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

# Kaleidoscope-Papageno: Turn your keyboard into a magical instrument

## Abstract

Define multi-key tap-dances (key-sequences), chords, key-clusters a mixture of it all. [Papageno](https://github.com/noseglasses/papageno/) an advanced pattern matching library seamlessly integrates with Kaleidoscope to provide ultimate flexibility in the generation of multi-key gestures/key patterns. An automatized compile-based compression mechanism converts key-patterns to C/C++ code that represents an optimized, statically allocated search tree. Key patterns are specified directly in the firmware sketch, written in an easy to use domain specific language.

## Motivation

### Utilization of thumb keys on ergonomic keyboards

While typing on traditional keyboards, the thumbs are chronically under-utilized. Most modern ergonomic keyboards therefore come with dedicated thumb key clusters.
As our thumbs are the most powerful fingers, it is desirable to shift as much work as possible from the other fingers to the thumbs. The actual number of (reachable) thumb keys varies among different keyboard designs. Typically, there are between four and eight (reachable) programmable thumb keys available.
This restricted number of keys means that with traditional key-codes the amount of different functions that can be triggered by hitting thumb keys is quite limited.
In the recent past there emerged quite a number of innovative ideas that aim to assign more than two key-codes or actions to a physical key.

### Existing Kaleidoscope-features

Using Kaleidoscope, tap-dances, modifier keys, with and without one-touch, as well as multiple key-map-layers provide different ways to creatively assign work to thumb keys. Every one of these methods has its specific advantages and downsides. A thorough analysis of the many possible ways to combine those features could fill an entire research article of its own. Therefore, we will concentrate on tap-dances to explain the enhancements introduced by Papageno.

Tap-dances theoretically allow to assign an infinite number of meanings to a single physical key. A certain number of key-presses, usually entered in rapid succession, triggers a specific action.
This is probably the only, but also the greatest disadvantage of *traditional* tap-dances. Hitting a key an exact number of times in a row can be fairly difficult to accomplish, especially for non pianists and when there are action definitions for say, three, four and five strokes of the same key.

### Towards multi-key tap-dances

As a keyboard comes with a multitude of keys, why not extend the concept of tap-dances to combinations of several keys? It is much easier to hit two, three or more keys in a defined order than to hit the same key a given number of times. Isn't entering key sequences, i.e. words precisely what typists are trained to do?

### Some numbers

Let's return to our initial motivation, shifting work to thumb keys. A basic idea could be to assign actions to two-key combinations (two key words) of thumb keys that are assigned to the left and the right hand.
For a keyboard with two thumb keys for each hand, this would mean that we would end up with eight possible two-key combinations of thumb keys assigned to different hands. Hereby, we assume that hitting two keys A and B in two different orders is supposed to trigger two different actions.

If we add to these eight combinations the four possible actions assigned to keys when hit independently, this sums up to twelve different actions for only four thumb keys.

It is up to the reader to solve the task of computing the drastic increase in number if we would also consider three-key combinations and so forth. And three key combinations are still easily doable.

### A real-life use case

Let's look at a brief example how multi-key tap-dances/sequences (in Papageno jargon called single-note lines, clusters and chords) could be applied to enhance Kaleidoscope's capability.

Imagine editing some sort of program source code. Hereby, a common task is to indent lines, either to improve readability or because the language requires indentation as part of its semantic (Python). Often it is also necessary to un-indent lines of code. Most editors assign shortcuts to both operations that are more or less simple to enter.
As indentation/un-indentation are performed quite frequently when programming, it appears to be a good idea to trigger them by using thumb-keys.

In the following we use the letter `A` for one of the left hand thumb-keys and `B` for one of the right hand thumb keys.
One possible solution for the given task would be to add indent
 when keys `A` and `B` are hit in order `A-B` and un-indent when hit in order `B-A`. Both operations would, of course, be programmed to emit the dedicated shortcut.

From a neuromuscular point of view this key-assignment strategy is doubtlessly a good choice as it is fairly easy to learn. Moreover, it is rather intuitive as we assign a forward-backward relation to indentation and un-indentation with respect to the order of the two thumb keys being hit. This certainly aids memorizing our new key assignment.

### Papageno - a pattern matching library

To bring the above example to life, we need a mechanism that is capable to recognize more or less complex keystroke patterns. The requirements described, among others,
led to the development of [Papageno](https://github.com/noseglasses/papageno/), an advanced pattern matching library. Originally implemented as part of Kaleidoscope, Papageno emerged to a stand-alone multi-platform library that via this plugin seamlessly integrates with Kaleidoscope. It provides a variety of different ways to define patterns that go far beyond advanced tap-dances only.

## Papageno as a Kaleidoscope plugin

Papageno allows for the definition of general multi-key patterns. All of the different patterns that are supported the same efficient and optimized pattern matching algorithm.

The heart of Papageno is a search tree that is established based on the definitions that are added to the firmware sketch file and compiled by Papageno's Glockenspiel compiler.

## Compatibility with existing Kaleidoscope features

The efficiency of Papageno's tree based pattern matching approach is one of the reasons why some features that were already part of Kaleidoscope, such as tap-dances and leader sequences, have been incorporated into Papageno \[Another reason is that they were just fun to implement based on Papageno ;-)\].

If features provided by Papageno are used in common with equivalent pre-existing Kaleidoscope-features, such as e.g. tap dances, resource usage can expected to be significantly higher than if Papageno would deal with the detection of tap dances alone. The reason is obvious: more program memory and probably more RAM is required to store binary code and variables for two or more plugins instead of just one.

Although optimized resource utilization is never a bad idea, Papageno plays along well with other Kaleidoscope features. Stock plugins that have been found to work well with Kaleidoscope-Papageno are

* Kaleidoscope-OneShot

If a plugin is not (yet) listed, this doesn't mean that it does not play along with Kaleidoscope-Papageno but simply that it has not been tested.
If a plugin is found to be incompatible, I will list it here as well.

Please feel free to generate issue reports when you encounter any incompatibilities with other plugins.

## Papageno as a wrapper to Kaleidoscope

Papageno-Kaleidoscope is designed as a wrapper for the rest of Kaleidoscope. Any keystrokes are intercepted and passed through Papageno's pattern matching engine. Only if a series of keystrokes is identified as not being part of any defined pattern, it is passed back to the main Kaleidoscope engine and, thus, to other plugins.

This allows for a strong decoupling of Papageno and Kaleidoscope, which is robust and can be expected to cooperate quite well with most other Kaleidoscope plugins.

For compatibility reasons, Papageno provides a layer mechanism that is fairly similar to that of Kaleidoscope's layered key-maps. At their point of definition, patterns are associated with layers. They are only active while their associated layer is the currently highest Kaleidoscope layer.
Layer fall through (see Papageno's documentation), works similar to the assignment of transparent key-codes in Kaleidoscope key-maps.

It is most common to emit Kaleidoscope key-codes or key-events (matrix row/column) when a defined pattern matches a series of keystrokes.
To allow for more advanced functionality, Papageno provides arbitrary user callback functions to be used as actions which, to enable further customization, can be supplied with user defined data.

## Prerequisites

As Kaleidoscope-Papageno builds and uses the *Glockenspiel* compiler that is part of the Papageno project, it requires extended build steps
to be carried out. Such build steps can currently not be incorporated in the stock build system (Kaleidoscope-Builder). This is why Kaleidoscope-Papageno relies on [Leidokos-CMake](https://github.com/CapeLeidokos/Leidokos-CMake) as its current build system.

## Usage

### Building the plugin

The following steps let you build and test Kaleidoscope-Papageno with a custom firmware. The general procedure
is similar to the general build procedure described in the Leidokos-CMake README with some slight modifications.

1. [Leidokos-CMake build step 1](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Prepare the build directory)

2. [Leidokos-CMake build step 2](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Clone the Leidokos-CMake repository)

3. Prepare additional plugins

        ```bash
        # Clone Kaleidoscope-Papageno
        cd TARGET_DIR/hardware/keyboardio/avr/libraries
        git clone --recursive https://github.com/noseglasses/Kaleidoscope-Papageno.git

        # Switch to your own firmware mod
        cd TARGET_DIR/hardware/keyboardio/avr/libraries/Model01-Firmware
        git remote add <your_firmware_remote_name> <your_firmware_remote_URL>
        git fetch <your_firmware_remote_name>
        git checkout <your_firmware_remote_branch>
        ```

4. [Leidokos-CMake build step 3](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Setup the CMake build system)

    Make sure that your keyboard is plugged in before you repeat this step, so the keyboard (port and stuff) will be recognized by the build system.

5. [Leidokos-CMake build step 4](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Build)

    You can also run [parallel builds](https://github.com/CapeLeidokos/Leidokos-CMake#parallel-builds).

    ___Important:___ For your firmware sketch to build with Kaleidoscope-Papageno,
    it must contain at least an empty
  ```
  glockenspiel_begin
  glockenspiel_end
  ```
  clause somewhere in the sketch file. If this is missing the build will fail.
  Read further to find out about the meaning of this clause.

6. Upload

  ```bash
  cmake --build . --target upload
  ```

  If the upload did not succeed, it is likely that your keyboard was not recognized by the build system. If this
  happened, no problem. Just unplug and replug the keyboard, then repeat steps 4 and 6.

### Preparing the sketch

To use Kaleidoscope-Papageno in your firmware, include its main header at
the top of your sketch.
```cpp
#include "Kaleidoscope-Papageno.h"
```
If you are going to define user function for customized actions, you need to define the pre-processor macro `KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS` before you include the main header. Explanations about what an action will follow.

```cpp
#define KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS
#include "Kaleidoscope-Papageno.h"
```

Kaleidoscope-Papageno is somewhat special, compared to other Kaleidoscope plugins, as it
needs to be the first in the list of plugins that are passed to `Kaleidoscope.use(...)`.

```cpp
void setup() {

  // First, call Kaleidoscope's setup function.
  Kaleidoscope.setup();

  // Register plugins.
  Kaleidoscope.use(
    &Papageno
    // ... other plugins
  )
}
```

The order in which the plugins are registered with `Kaleidoscope.use(...)`
decides on the order the plugins are internally called by Kaleidoscope's core. to be on the safe side, it is important to let Kaleidoscope-Papageno to come first.
There might be other plugins for which it is necessary to be processed before
Kaleidoscope-Papageno. Just play with the order and see what happens.

Another difference to other plugins is that Kaleidoscope-Papageno comes
with its own `loop()` method. This must be called instead of Kaleidoscope's default
`loop()` method.

```cpp
void loop() {
  Papageno.loop();
}
```

If you defined the macro `KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS`
before including `Kaleidoscope-Papageno.h`, you need to add a small amount
of boiler plate code to the very end of your sketch file, namely
```cpp
extern "C" {
#include "Kaleidoscope-Papageno-Sketch.hpp"
}
```

This ensures that the C/C++ code that is generated by Glockenspiel, based on the pattern definitions in the sketch, is
included after the definitions of any user functions that are supposed to serve as pattern actions. This is important as
such user functions would otherwise be reported as "undefined symbols"
when compiling the firmware.

### Defining Papageno patterns

The definition of patterns is based on a domain specific language that is converted to a highly efficient C/C++ code
by the Glockenspiel compiler. Pattern definitions can be added directly to the Kaleidoscope sketch either in a commented region
```cpp
/* Papageno definitions follow

glockenspiel_begin
...
glockenspiel_end
*/
```

or surrounded by `#if 0 ... #endif`.
```cpp
#if 0
Papageno definitions follow

glockenspiel_begin
...
glockenspiel_end
#endif
```
All Papageno definitions must be surrounded by `glockenspiel_begin` and
`glockenspiel_end` as starting and ending tags to enable the Glockenspiel compiler to distinguish between Papageno definitions and C++ code.

Papageno definitions may occur at any place in the sketch. Only one block of Papageno definitions is allowed.

### Defining inputs

Kaleidoscope-Papageno checks keyboard input and tries to recognize patterns.
For improved performance of pattern matching, only keys that are explicitly
registered with Papageno may be part of patterns. Keys are thereby defined by their position (row/column) in the keyboard matrix. In Papageno jargon, keys that participate in pattern matching are called *inputs*. One can define as much as 255 separate inputs. In reality it depends on the number of keys of the keyboard, how many different inputs actually make sense. The maximum amount is the number of different physical keys.

All the following examples assume a Keyboardio Model01 keyboard.

An input can e.g. be defined as follows.
```
input: LeftThumb1 <KEYPOS> =  $ 0,  7 $
```
This would allow the leftmost thumb key of the Model01 to be used as part of a pattern.
The thumb key can further on be referenced by the name `LeftThumb1`. The string `$ 0,  7 $` defines the key's matrix
position as `row = 0, col = 7`.

### Defining actions

Before we define key patterns, we should concern ourselves with question what is supposed to happen once a pattern matches. Kaleidoscope-Papageno supports different types of actions that are going to be explained in the following.

#### Simple keycode actions

The most common action is to trigger a key event with a given keycode.
```
action: Key_C <KEYCODE>
```
Unsurprisingly, this will cause the printable key "C" be emitted once a
pattern matches. As this type of actions is pretty common, `KEYCODE` actions for  keycodes that are defined by the stock firmware are define in the  
file `glockenspiel/predefines.gls` in the Kaleidoscope-Papageno repository.
This file is included by Kaleidoscope-Papageno during the Glockenspiel
compile. Thus, for standard keys, keycode actions just work out of the
box. Be careful not to explicitly define actions for standard keys explicitly as those would collide with the definitions in `glockenspiel/predefines.gls` and therefore cause a Glockenspiel compiler errors.

If you want to use a non standard keycode alias which you defined in
your C++ code somewhere (directly within the sketch file or included therein), you
can define an explicit action.
```
action: my_Special_Key <KEYCODE>
```
___Note:___ `my_Special_Key` must either be a valid C++ symbol or pre-processor macro.

#### Complex keycode actions

Of course, it is also possible to define complex keycodes as actions, e.g. a printable keys with active modifiers.
```
action: shiftCtrlC <COMPLEX_KEYCODE> =  $ LCTRL(LSHIFT(Key_C)) $
```
The above action can be referenced by the name `shiftCtrlC` when defining patterns.

#### Matrix key position actions

Another type of action is one that emits a key event at a given matrix position.
This fools the firmware to assume that the respective key on the keyboard is  physically tapped.

```
action: pressLED <KEYPOS> = $ 0, 6 $
```
This action causes a key tap at position `row=0, col`.

### User functions as actions

The most general type of actions that are supported by Kaleidoscope-Papageno are user functions. A user function must have the following signature.
```cpp
void user_callback(PPG_Count activation_flags, void *user_data)
```

A user function is defined as follows.
```
action: doubleTab <USER_FUNCTION> =  $ doubleTabCB, NULL $
```

This example defines an action that causes the tab key to be tapped twice.

The callback function could read:
```cpp
void doubleTabCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION

   handleKeyswitchEvent(Key_Tab, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();

   handleKeyswitchEvent(Key_Tab, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();
}
```
Please note the invokation of the two macros `PPG_CALLBACK_NO_REPEAT` and `PPG_CALLBACK_ONLY_ACTIVATION` at the top of the user function.

`PPG_CALLBACK_NO_REPEAT` marks the function to be called only once. If this macro wasn't there, the function would be called once in every firmware loop cycle as
long as the last key is held that caused the pattern to match. It depends on the application case whether this is desired. Key repeat
can be useful in some cases, in others not.

`PPG_CALLBACK_ONLY_ACTIVATION` causes the user function only to be called
once when the pattern matches. If this macro is not used, the user function would be called another time when the key that caused the pattern to match is released.

___Important:___ Both of the above macros assume the first function argument of the user function to be called `activation_flags`.

The callback function can be passed an additional `void *user_data` call argument. This is data that is to be passed once the function is invoked for a specific
action. The user data that is passed to the actual call is defined in the `$ ... $` clause at the end of the action definition line. If you don't require any `user_data` just pass `NULL`.

As an example, the `user_data` pointer could be used to pass a `Key` value to a user function, e.g. a function that adds the left shift modifier to the key that is passed.
```cpp
void shiftedKeyCB(PPG_Count activation_flags, void *user_data)
{
   PPG_CALLBACK_NO_REPEAT
   PPG_CALLBACK_ONLY_ACTIVATION

   Key k;
   k.raw = (uint16_t)user_data;

   handleKeyswitchEvent(LSHIFT(k), UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();

   handleKeyswitchEvent(LSHIFT(k), UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED);
   kaleidoscope::hid::sendKeyboardReport();
}

```

```
action: shifted_A <USER_FUNCTION> =  $ shiftedKeyCB, (void*)Key_A.raw $
```
This works due to the fact the `Key` struct on an atmega32u4 (used by the Keyboardio Model01) is a two-byte variable that thus has the same size as `void *`. This means that we pass the `Key` by value.

If you are not sure about the size of a variable that you want to pass to a user function, it is safest to rather pass by reference, i.e. passing the pointer to the variable after casting it to `void*`. But be careful not to pass pointers to local variables, which could easily lead to a crash of the entire firmware.

### Referencing keys or actions by alias

Sometimes it is convenient to assign alias names to inputs or actions.
```
alias: LeftOutermostThumb = LeftThumb1
alias: myActionAlias = shifted_A
```
In the above example an alias name `LeftOutermostThumb` is assigned to an input `LeftThumb1` and another alias to an already defined action.

### Defining patterns

#### Multi key sequences

A multi key sequence is defined as a set of keys that have to be hit in
a precise order for the pattern to match.

Let's assume we have defined two inputs `LeftThumb1` and `RightThumb1`. We want
an action to be called if they are hit one after the other.
```
|RightThumb1| -> |LeftThumb1| : Key_Delete
```
This would cause `Key_Delete` to be emitted when the pattern matches.

Of course, you can have key sequences with more than two keys if you want. Just add more inputs separated by `->`.

In Papageno's jargon `|RightThumb1|` is called a *note* and the entire key sequence a *single note line*. This is because of the analogy to playing a piano.

#### Key clusters

Key clusters are related to key sequences with the slight difference that
the inputs can be activated in an arbitrary order. The only condition for the cluster to match is that all listed inputs (keys) have been activated (pressed) at least once.

A cluster of two thumb keys that triggers `Key_Enter`can be defined as follows. We hereby assume that `LeftThumb3` and `RightThumb2` have both already been defined as inputs.
```
{LeftThumb3, RightThumb2} : Key_Enter
```

#### Key chords

Key chords are more strict than clusters. They require all listed inputs
to be activated at the same time for the action to be triggered.
```
[LeftThumb3, RightThumb2] : Key_Enter
```

Please note the different brackets compared to the cluster example.

#### Tap dances

Tap dances work the same way as known from plugin [Kaleidoscope-TapDance](https://github.com/keyboardio/Kaleidoscope-TapDance).

If a key is supposed to be tapped multiple times to trigger an action, you can
do so easily.
```
|Special1|*2 : repeatLastCommand
```
When the input `Special1` if activated (tapped) two times, the action `repeatLastCommand` will be triggered.

It is common to assign different actions to a single key, depending on how often it is tapped. Say, we want to issue an ordinary string search of our text editor (user function `ordinarySearch`) in the current file once we hit key `Special3` twice, but a search across all files (user function `fileSearch`) when we hit the same key three times.
```
|Special3|*3 : ordinarySearch@2, fileSearch@3
```
The maximum amount of taps (here 3) must be added after the definition of the token (note, cluster, chord) (`|Special3|*3`). The amount of taps that are associated with the different actions must follow the `@`.

#### Extended tap dances

Following the idea of tap dances that require a key to be hit several times,
we can also define more complex "dances" that require whole patterns to be repeated.

Assume you want to flash your keyboard once in a while with the latest firmware build. There's no need to unplug and replug the keyboard every time you flash. You can trigger flashing by means of the Model01's `rebootBootloader()` function. For this to be convenient but also safe from accidentally being caused, we define a user function.
```cpp
void rebootCB(PPG_Count activation_flags, void *user_data)
{
   KeyboardHardware.rebootBootloader();
}
```
This function we want to trigger through a magic key combination that is unlikely to be issued. Hitting LED and ANY twice in a row might be unlikely enough to happen by accident.
```
action: reboot <USER_FUNCTION> = $ rebootCB, NULL $
phrase: reboot_phrase = |my_Key_LED| -> |my_Key_ANY|
#reboot_phrase*2 : reboot
```

By using Glockenspiel's `#` operator, we "dereference" the phrase name `reboot_phrase` and require it to be entered twice for the action to be triggered.

#### Leader sequences
TODO
