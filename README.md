![status][st:experimental] [![Build Status][travis:image]][travis:status]

[travis:image]: https://travis-ci.org/noseglasses/Kaleidoscope-Papageno.svg?branch=master
[travis:status]: https://travis-ci.org/noseglasses/Kaleidoscope-Papageno

[st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
[st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
[st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

# Kaleidoscope-Papageno

__Turn your keyboard into a magical instrument!__

Define multi-key tap-dances (key-sequences), chords, key-clusters, leader sequences or a mixture of it all. Via this plugin, [Papageno](https://github.com/noseglasses/papageno/) an advanced pattern matching library seamlessly integrates with Kaleidoscope to provide ultimate flexibility in the generation of multi-key gestures/key patterns. An automatized compile-based compression mechanism converts key-patterns to C/C++ code that represents an optimized, statically allocated search tree. Key patterns are specified directly in the firmware sketch, written in an easy to use domain specific language.

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Kaleidoscope-Papageno](#kaleidoscope-papageno)
- [Motivation](#motivation)
	- [Utilization of thumb keys on ergonomic keyboards](#utilization-of-thumb-keys-on-ergonomic-keyboards)
	- [Existing Kaleidoscope-features](#existing-kaleidoscope-features)
	- [Towards multi-key tap-dances](#towards-multi-key-tap-dances)
	- [Some numbers](#some-numbers)
	- [A real-life use case](#a-real-life-use-case)
- [Papageno - a pattern matching library](#papageno-a-pattern-matching-library)
	- [Compatibility with existing Kaleidoscope features](#compatibility-with-existing-kaleidoscope-features)
	- [Papageno as a wrapper to Kaleidoscope](#papageno-as-a-wrapper-to-kaleidoscope)
- [Prerequisites](#prerequisites)
- [Preparing the sketch](#preparing-the-sketch)
	- [Defining Papageno patterns](#defining-papageno-patterns)
	- [Comments in Papageno/Glockenspiel code](#comments-in-papagenoglockenspiel-code)
	- [Defining inputs](#defining-inputs)
	- [Defining actions](#defining-actions)
		- [Simple keycode actions](#simple-keycode-actions)
		- [Complex keycode actions](#complex-keycode-actions)
		- [Matrix key position actions](#matrix-key-position-actions)
		- [User functions as actions](#user-functions-as-actions)
	- [Referencing keys or actions by alias](#referencing-keys-or-actions-by-alias)
	- [Defining patterns](#defining-patterns)
		- [Key sequences](#key-sequences)
		- [Key clusters](#key-clusters)
		- [Key chords](#key-chords)
		- [Tap dances](#tap-dances)
		- [Phrases](#phrases)
		- [Extended tap dances - dance with phrases](#extended-tap-dances-dance-with-phrases)
		- [Leader sequences](#leader-sequences)
		- [Sequence strings instead of key sequences (single note lines)](#sequence-strings-instead-of-key-sequences-single-note-lines)
	- [Timeout](#timeout)
	- [Action fallback](#action-fallback)
	- [Layers](#layers)
	- [The `$...$` syntax](#the-syntax)
- [Building the plugin](#building-the-plugin)

<!-- /TOC -->

# Motivation

## Utilization of thumb keys on ergonomic keyboards

While typing on traditional keyboards, the thumbs are chronically under-utilized. Most modern ergonomic keyboards therefore come with dedicated thumb key clusters.
As our thumbs are the most powerful fingers, it is desirable to shift as much work as possible from the other fingers to the thumbs. The actual number of (reachable) thumb keys varies among different keyboard designs. Typically, there are between four and eight (reachable) programmable thumb keys available.
Due to this restricted number, the amount of different functions that can be triggered by assigning traditional key-codes is quite limited.
In the recent years quite a number of innovative ideas emerged that all aim to assign more than two key-codes or actions to one physical key. Kaleidoscope-Papageno reinterprets some of these ideas to provide new unique functionality.

## Existing Kaleidoscope-features

Kaleidoscope already provides different means to assign additional functionality to a single key, e.g. tap-dances, modifier keys, with and without one-touch, as well as the use of multiple key-map-layers. Every one of these methods has its specific advantages and downsides. A thorough analysis of the many possible ways to combine those features could fill entire research articles. We will, thus, concentrate on tap-dances to explain the enhancements introduced by Kaleidoscope-Papageno.

[Tap-dances](https://github.com/keyboardio/Kaleidoscope-TapDance) theoretically allow to assign an infinite number of different actions to a single physical key. A certain number of key-presses, usually entered in rapid succession, triggers a specific action.
Although very flexible, tap-dances come with on disadvantage. Hitting a key an exact number of times in a row can be fairly difficult to accomplish, especially for non pianists and when there are different actions assigned to say, three, four and five strokes of the same key.

## Towards multi-key tap-dances

As a keyboard comes with a multitude of keys, why not extend the concept of tap-dances to combinations of several keys? It is much easier to hit two, three or more different keys in a defined order than it is to hit the same key a given number of times. Isn't typing key sequences, i.e. words precisely what typists are trained to do?

## Some numbers

Let's return to our initial motivation, shifting work to thumb keys. A basic idea could be to assign actions to two-key combinations of thumb keys, always one key of the left and one key of the right hand.
For a keyboard with two thumb keys for each hand, this would mean that we would end up with eight possible two-key combinations. Hereby, we assume that hitting two keys A and B in two different orders is supposed to trigger two different actions.

If we add to these eight combinations the four possible actions assigned to the keys when hit alone, this sums up to twelve different actions for only four thumb keys.

The number of possible actions drastically increases if we also consider three-key combinations and so forth. Even if it sounds complex at this point - three key combinations are absolutely doable.

## A real-life use case

Let's look at a brief example how multi-key tap-dances/sequences (in Papageno jargon called single-note lines, clusters and chords) could be applied to enhance Kaleidoscope's capability.

Imagine, you edit some sort of program source code. It is hereby a common task is to indent/un-indent lines. Most editors assign shortcuts to both operations that are more or less easy to trigger.
As indentation/un-indentation are performed quite frequently when programming, why not assign them to our thumb keys?

In the following, letter `A` represents one of the left hand thumb-keys and `B` one of the right hand thumb keys.
One possible solution for the given task would be to add indent
 when keys `A` and `B` are hit in order `A-B` and remove indent when hit in order `B-A`.

From a neuromuscular point of view this task-assignment strategy is doubtlessly a good choice as both gestures are fairly easy to learn. It is that intuitive, because we assign a forward-backward relation to indentation and un-indentation with respect to the order of the two thumb keys being hit. This certainly aids memorizing our new shortcut-key assignment.

# Papageno - a pattern matching library

To bring the above example to life, we need a mechanism that is capable to recognize more or less complex keystroke patterns. The requirements described above, were the initial motivation of the advanced pattern matching library [Papageno](https://github.com/noseglasses/papageno/). It provides a variety of different ways to define patterns that go far beyond advanced tap-dances only.

## Compatibility with existing Kaleidoscope features

The efficiency of Papageno's tree based pattern matching approach is one of the reasons why some features that were already part of Kaleidoscope, such as tap-dances and leader sequences, have been incorporated into Papageno \[Another reason is that they were just fun to implement ;-) \].

If features provided by Papageno are used in common with other Kaleidoscope plugins, such as e.g. tap dances, resource usage can expected to be significantly higher than if Papageno would deal with the detection of tap dances alone. The reason is obvious: more program memory and probably more RAM is required to store binary code and variables for two or more plugins instead of just one.

Although optimized resource utilization is never a bad idea, Papageno plays along well with other Kaleidoscope features. Stock plugins that have been found to work well with Kaleidoscope-Papageno are

* Kaleidoscope-OneShot.

If a plugin is not listed above, this doesn't mean that it is incompatible with Kaleidoscope-Papageno but simply that it has not been tested yet.
If a plugin is found to be incompatible, I will list it here as well.

Please feel free to submit issue reports when you encounter any incompatibilities with other plugins - and also if you are sure about an an explicit compatibility. Both will be listed here to inform other users.

## Papageno as a wrapper to Kaleidoscope

Papageno-Kaleidoscope is currently designed to be a wrapper for the rest of Kaleidoscope. It intercepts some keystrokes and passes them through Papageno's pattern matching engine. Only if a of keystrokes is identified as not being part of any defined pattern, it is passed through to the Kaleidoscope core, and thus, to other plugins.

For compatibility reasons, Papageno provides a layer mechanism that is fairly similar to that of Kaleidoscope's layered key-maps. At their point of definition, patterns are associated with layers. They are only active while their associated layer is the currently highest Kaleidoscope layer.
Layer fall through (see Papageno's documentation), works similar to the assignment of transparent key-codes in Kaleidoscope key-maps.

It is most common to emit Kaleidoscope key-codes or key-events (matrix row/column) when a defined pattern matches a series of keystrokes. Papageno also allows arbitrary user callback functions to be used as actions, which can be supplied with user defined data to enable further customization.

# Prerequisites

As Kaleidoscope-Papageno uses the *Glockenspiel* compiler that is build as part of the Papageno project, it requires extended build steps. Such extended build steps can currently not be incorporated in the stock build system Kaleidoscope-Builder. This is why Kaleidoscope-Papageno must rely on [Leidokos-CMake](https://github.com/CapeLeidokos/Leidokos-CMake) as its current build system.

# Preparing the sketch

To use Kaleidoscope-Papageno with your firmware, you have to add some definition to your sketch file. Some of these definitions are probably nothing new to you as they are similar to what needs to be added bring in any other Kaleidoscope plugin. Other changes of the sketch, such as adding Papageno pattern definitions require some more explanations that will be provided in the following.

To use Kaleidoscope-Papageno in your firmware, first include its main header at
the top of your sketch.
```cpp
#include "Kaleidoscope-Papageno.h"
```
If you are going to define user functions to trigger custom actions, you need to define the pre-processor macro `KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS` before you include the main header. We will learn soon, what a user function action is.

```cpp
#define KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS
#include "Kaleidoscope-Papageno.h"
```

Kaleidoscope-Papageno is somewhat special, compared to other Kaleidoscope plugins, as it
needs to be the first in the list of plugins that are passed to `Kaleidoscope.use(...)` in Arduino's `setup()` function.

```cpp
void setup() {

  // First, call Kaleidoscope's setup function.
  Kaleidoscope.setup();

  // Register plugins.
  Kaleidoscope.use(
    &Papageno // Let Papageno appear as first plugin
    // ... other plugins
  )
}
```

Explanation: The order in which the plugins are registered with `Kaleidoscope.use(...)`
decides on the order the plugins are internally called by Kaleidoscope's core, e.g. when processing keyboard events.

There might be other plugins for which it is necessary to be processed before
Kaleidoscope-Papageno. Just play with the order you add the plugins to `Kaleidoscope.use(...)` and see what happens.

Another difference to other plugins is that Kaleidoscope-Papageno comes
with its own `loop()` method. This must be called instead of Kaleidoscope's default
`loop()` method.

```cpp
void loop() {
  Papageno.loop();
}
```

Finally, if you defined the macro `KALEIDOSCOPE_PAPAGENO_HAVE_USER_FUNCTIONS`
before including `Kaleidoscope-Papageno.h`, you need to add a small amount
of boiler plate code to the very end of your sketch file, namely

```cpp
extern "C" {
#include "Kaleidoscope-Papageno-Sketch.hpp"
}
```

This ensures that the C/C++ code that is generated by Glockenspiel, based on the pattern definitions in the sketch, is
included __after__ the definitions of any user functions that are supposed to serve as pattern actions. This is important as
such user functions would otherwise be reported as "undefined symbols"
when compiling the firmware.

Now you are almost done with preparing the sketch. The only thing that's
missing is the definition of Papageno patterns.

## Defining Papageno patterns

The definition of patterns is written in Papageno/Glockenspiel's own domain specific language that it converts to highly efficient C/C++ code during the compile. Pattern definitions are added directly to the Kaleidoscope sketch either in a commented region
```cpp
/* Papageno definitions follow

glockenspiel_begin
... patterns go here ...
glockenspiel_end
*/
```

or surrounded by `#if 0 ... #endif`.
```cpp
#if 0
Papageno definitions follow

glockenspiel_begin
... patterns go here ...
glockenspiel_end
#endif
```
All Papageno definitions must be surrounded by `glockenspiel_begin` and
`glockenspiel_end` as starting and ending tags to enable the Glockenspiel compiler to distinguish between Papageno definitions and the sketch's remaining C++ code.

Papageno definitions may occur at any place in the sketch, but only one `glockenspiel_begin ... glockenspiel_end` block is allowed.

## Comments in Papageno/Glockenspiel code

Everything that follows a `%` until the end of a line is considered part of the comment.
```
/*
glockenspiel_begin

% This is a comment.
input: an_input <KEYPOS> = $ 1, 3 $ % This is a commented input.

glockenspiel_end
*/
```

## Defining inputs

Kaleidoscope-Papageno checks your keyboard input and attempts to recognize patterns while you are typing.
For improved performance of pattern matching, only keys that are explicitly
registered with Papageno may be part of such patterns. Keys are thereby defined by their position (row/column) in the keyboard matrix. In Papageno jargon, keys that participate in pattern matching are called *inputs*. In theory, one can define as much as 255 separate inputs with Kaleidoscope-Papageno. In reality, how many different inputs actually make sense, depends on the actual number of keys on the keyboard. The maximum amount of usable inputs is limited by the number of different physical keys.

Arduino hackers: In theory it is possible to attach other devices to the Arduino board that could serve as extended inputs. Currently this is not supported by Kaleidoscope-Papageno, but if you have an enhancement idea, feel free to open an issue report on GitHub and describe your ideas.

All the following examples assume a Keyboardio Model01 keyboard is being used.

An input for a key at position `row = 0, col = 7` can be defined as follows and named `LeftThumb1`.
```
input: LeftThumb1 <KEYPOS> =  $ 0,  7 $
```

For those using a Kaleidoscope Model01, there is an [online keymap](http://www.keyboard-layout-editor.com/#/gists/208ec9c7f9a08382c101b558f1d983b1) that makes it quite easy to determine the matrix position for a specific key.

## Defining actions

Pattern matching is only useful, if we assign actions to matching patterns. Kaleidoscope-Papageno supports different types of actions that we are going to explained in the following.

### Simple keycode actions

The most common action is to emit a keyboard event with a given keycode.
```
action: Key_C <KEYCODE>
```
This will cause the printable key "C" to be emitted once a
pattern matches. As this type of action is pretty common, `KEYCODE`-actions for all keycodes that are defined by the stock firmware are define in the  
file `glockenspiel/predefines.gls` that lives in the Kaleidoscope-Papageno repository.

This file is included by Kaleidoscope-Papageno during the Glockenspiel
compile. This means, that for standard keys, keycode actions just work out of the
box.

Please, be careful not to explicitly define actions for any of the standard keys that are already listed in `glockenspiel/predefines.gls`. Otherwise, Glockenspiel compiler errors would result.

If you want to use a non standard keycode alias which you defined in
your C++ code somewhere (directly within the sketch file or included therein), you
can define an explicit action.
```
constexpr Key my_Special_Key = LCTRL(Key_A);
/*
glockenspiel_begin
...
action: my_Special_Key <KEYCODE>
...
glockenspiel_end
*/
```
___Note:___ `my_Special_Key` could either be a valid C++ symbol or a pre-processor macro.

### Complex keycode actions

It is also possible to define complex keycodes as actions within the Papageno definitions, e.g. a printable key with an active modifier.
```
action: shiftCtrlC <COMPLEX_KEYCODE> =  $ LCTRL(LSHIFT(Key_C)) $
```
The above action can be referenced by the name `shiftCtrlC` when defining patterns.

### Matrix key position actions

A third possible type of action is one that emits a key event at a given keyboard matrix position.
This fools the firmware to assuming that the respective key on the keyboard is physically tapped.

```
action: pressLED <KEYPOS> = $ 0, 6 $
```
This action causes a key tap at position `row=0, col=6`.

### User functions as actions

Finally, the most general type of actions that are supported by Kaleidoscope-Papageno are user functions.

A user function must have the following signature.
```cpp
void user_callback(PPG_Count activation_flags, void *user_data)
```

A user function action may e.g. be defined as follows.
```
action: doubleTab <USER_FUNCTION> =  $ doubleTabCB, NULL $
```

It defines an action that makes the tab-key to be tapped twice when the a pattern matches that the action is assigned to.

An appropriate callback function could then read:
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
Please note the invokation of the two macros `PPG_CALLBACK_NO_REPEAT` and `PPG_CALLBACK_ONLY_ACTIVATION` at the top of the callback.

`PPG_CALLBACK_NO_REPEAT` informs Kaleidoscope-Papageno that the function wants to be called only once per match. If this macro wasn't there, the function would be called once in every firmware loop cycle as
long as the last key is held, that finally caused the pattern match. It depends on the application case whether this is desired behavior. Key repeat
can be useful in some cases, in others not.

`PPG_CALLBACK_ONLY_ACTIVATION` causes the user function only to be called
when the pattern matches. Without this macro, the user function would be called a second time when the key, that caused the pattern match, is released.

___Important:___ Both of the above macros require the first function argument of the user function to be called `activation_flags`.

An action callback function must be passed an additional call argument `void *user_data`. This can be used to pass data to the callback function that is specific to the actual action. This is only relevant if an action callback is shared by different action definitions. The user data that is supposed to be passed is defined in the `$ ... $` clause at the end of the action definition line. If you don't require any `user_data`, just pass `NULL`.

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
...
/*
glockenspiel_begin
...
action: shifted_A <USER_FUNCTION> =  $ shiftedKeyCB, (void*)Key_A.raw $
...
glockenspiel_end
*/
```
This passes the key by reference, which works due to the fact the `Key` struct on an atmega32u4 (used by the Keyboardio Model01) is a two-byte variable that has the exact same size as `void *`.

If you are not sure about the size of a variable that you want to pass to a user function, it is safest to pass by-reference, instead of by-value. Pass a pointer to a variable after casting it to `void*`. But be careful not to pass pointers to local variables. This could easily crash the firmware.

## Referencing keys or actions by alias

Sometimes it is convenient to assign alias names to inputs or actions.
```
alias: LeftOutermostThumb = LeftThumb1
alias: myActionAlias = shifted_A
```
In the above example an alias name `LeftOutermostThumb` is assigned to input `LeftThumb1` and another alias to an already defined action.

## Defining patterns

Patterns are defined by ordering tokens (notes, clusters, chords). Thereby
tokens are separated by Glockenpiel's `->` operator.

### Key sequences

A key sequence is defined as a set of keys that have to be pressed in
a precise order for the pattern to match.

Let's assume we have defined two inputs (keys) `LeftThumb1` and `RightThumb1`.
```
|RightThumb1| -> |LeftThumb1| : Key_Delete
```
This would cause `Key_Delete` to be emitted when the pattern matches.

Of course, you can have key sequences with more than two keys if you want. Just add more inputs separated by `->`.

In Papageno's jargon `|RightThumb1|` is called a *note* and the entire key sequence a *single note line*, thereby using an analogy to musical melodies.

### Key clusters

Key clusters are related to key sequences with the slight difference that
their inputs can be activated in an arbitrary order. A cluster matches once all associated inputs (keys) have been activated (pressed) at least once.

A cluster of two thumb keys that triggers `Key_Enter`can be defined as follows.
```
{LeftThumb3, RightThumb2} : Key_Enter
```
We hereby assume that `LeftThumb3` and `RightThumb2` have both already been defined as inputs.

### Key chords

Key chords are more strict than clusters. They require all listed inputs
to be activated at the same time for the token to match and the action to be triggered.
```
[LeftThumb3, RightThumb2] : Key_Enter
```

Please note the different brackets compared to the cluster example.

### Tap dances

Tap dances work the same way as known from plugin [Kaleidoscope-TapDance](https://github.com/keyboardio/Kaleidoscope-TapDance).

It is easy to define an action for a key that is supposed to be tapped multiple times.
```
|Special1|*2 : repeatLastCommand
```
When the input `Special1` if activated (tapped) two times, the action `repeatLastCommand` will be triggered.

__Important:__ The amout of taps that follows the `*` must be a literal integer constant. C/C++ symbols are not allowed here.

Using tap dances, it is common to assign different actions to a single key, depending on how often it is tapped. Suppose, we want to issue an ordinary string search of our text editor (user function `ordinarySearch`) in the current file once we hit key `Special3` twice, but a search across all files (user function `fileSearch`) when we hit the same key four times. The Papageno pattern definition would read as follows.
```
|Special3|*4 : ordinarySearch@2, fileSearch@4
```
The maximum amount of taps (here 4) must be added after the definition of the token (note, cluster, chord) (here `|Special3|`). The amount of taps that are associated with the different actions must follow an `@` after the name of the action to trigger.

### Phrases
If a sequence of tokens is used several times, it can be given a name by defining a phrase. A named phrase can be referenced using the `#` operator.
```
phrase: my_phrase = |Special1| -> |Special2|

|Special3| -> #my_phrase: an_action
|Special4| -> #my_phrase: an_action
```
This triggers `an_action` it either `Special3, Special1, Special2` or `Special4, Special1, Special2` are activated.

### Extended tap dances - dance with phrases

Following the idea of tap dances that require a key to be hit several times,
we can also define more complex "dances" that require whole phrases to be repeated.

Assume we want to flash our keyboard once in a while with the latest firmware build. There's no need to unplug and replug the keyboard every time we flash. We can trigger flashing by calling the Model01 hardware's `rebootBootloader()` function. For this to be convenient but also safe from accidentally being triggered, we define a user function and trigger it through a magic keyboard gesture.
```cpp
void rebootCB(PPG_Count activation_flags, void *user_data)
{
   KeyboardHardware.rebootBootloader();
}
```
Hitting LED and ANY twice in a row might be unlikely enough to happen by accident.
```
action: reboot <USER_FUNCTION> = $ rebootCB, NULL $
phrase: reboot_phrase = |my_Key_LED| -> |my_Key_ANY|

#reboot_phrase*2 : reboot
```

By using Glockenspiel's `#` operator, we "dereference" the phrase name `reboot_phrase` and require it to be entered twice for the action to be triggered.

### Leader sequences

Another feature, already known from [Kaleidoscope-Leader](https://github.com/keyboardio/Kaleidoscope-Leader). But Papageno's leader sequences are slightly more convenient and general.

To define a simple leader sequence, select an input as your leader input (key) and add a sequence string to the definition. A (input) sequence can be defined by a sequence of alphabetic characters [a-zA-Z]. Those are treated non-case sensitive, so it doesn't matter whether you write "abc", "aBc" or "ABC". Every character is used to lookup an input that has been assigned the same name (in lower case).

```
input: my_lead_Key <KEYPOS> = $ 0, 0 $
input: an_unrelated_Key <KEYPOS> = $ 2, 2 $

% Define and name those inputs that are supposed to be part of a sequence string.
input: a = $ 2, 1 $
input: b = $ 3, 5 $
input: c = $ 3, 3 $

% You can also use alias, if necessary.
alias: d = an_unrelated_Key

alias: my_abacabd_action = Key_Escape

|my_lead_Key| -> "abacabd" : my_abacabd_action % "abacabd" is the sequence string.
```

Whenever you press the lead key (here `my_lead_Key`), followed by the sequence of keys that matches `abacabd`, the action `my_abacabd_action` will be triggered. Pretty simple, isn't it?

You can also go without a lead key. Strictly speaking, due to the absence of the lead key, this is not a leader sequence anymore ;-)
```
"abacabd" : my_abacabd_action
```

As there are so many different possible alphabetic keyboard layouts (QWERTY, Dvorak, ...), we did not predefine any alphabetic input keys.

### Sequence strings instead of key sequences (single note lines)

Sequence strings are equivalent to defining a single note line. This means that
```
"abacabd" : my_abacabd_action
```
is equivalent to
```
|a| -> |b| -> |a| -> |c| -> |a| -> |b| -> |d| : my_abacabd_action
```

## Timeout
Kaleidoscope-Papageno supports a timeout that ends the current pattern recognition.
This timeout in `[ms]` can be defined as part of the Glockenspiel/Papageno code in your sketch.
```
default: event_timeout =  $ 200 $
```

## Action fallback

Actions can also be assigned to tokens that are not necessarily at the end of a
pattern, e.g.

```
|input_1| -> |input_2| : a_action -> |input_1| -> |input_2| : b_action
```

If keys `input_1, input_2` are pressed and then one of the following happens:

* timeout,
* any non-input key is pressed,
* any key is pressed that causes a pattern matching failure,

`a_action` is triggered.

If the full event sequence `input_1, input_2,input_1, input_2` occurs, `b_action` is triggered.

This is the same behavior as known from tap dances, generalized to token sequences.

## Layers

Keymap layers are an important concept in Kaleidoscope.
Kaleidoscope-Papageno may also be assigned to layers. Papageno's layer system is therefore linked with
that of Kaleidoscope. To define a pattern that is only active on layers above a
specific number, including it, you can write the following.
```
layer: 1
% all patterns that follow will be assigned to layer 1 and above
```
You can also use C/C++ identifiers for layers, say you use the C++ compile time
constants `primaryLayer` and `secondaryLayer` to define the lowest layer.
```cpp
constexpr int primaryLayer = 0;
constexpr int secondaryLayer = 1;
/*
glockenspiel_begin

... input and action definitions ...

layer: primaryLayer
|input_1| -> |input_2| : action_1 % on layer primaryLayer

layer: secondaryLayer
|input_2| -> |input_1| : action_2 % on layer secondaryLayer

glockenspiel_end
*/
```

## The `$...$` syntax

You might have wondered about the strange dollar signs that are used
in input and action definitions. These solve the task of delimiting definitions that are expected to be C/C++ code, such as the function name and `user_data` in user function action definitions. As `$` is not a valid symbol in C/C++ code, we selected it as a delimiting character to avoid ambiguities. Just think of it the same as if it was `"..."` or `'...'`.

# Building the plugin

The following steps let you build and test Kaleidoscope-Papageno with a custom firmware. The general procedure
is similar to the general build procedure described in the Leidokos-CMake README with some slight modifications.

1. Execute [Leidokos-CMake build step 1](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Prepare the build directory)

2. Execute [Leidokos-CMake build step 2](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Clone the Leidokos-CMake repository)

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

4. Execute [Leidokos-CMake build step 3](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Setup the CMake build system)

    Make sure that your keyboard is plugged in before you execute this step, so the keyboard (port and stuff...) will be recognized by the build system.

5. Execute [Leidokos-CMake build step 4](https://github.com/CapeLeidokos/Leidokos-CMake#usage) (Build)

    You can also use [parallel builds](https://github.com/CapeLeidokos/Leidokos-CMake#parallel-builds).

    ___Important:___ For your firmware to build with Kaleidoscope-Papageno,
    your sketch file must contain at least an empty clause.
    ```
    /*
    glockenspiel_begin
    glockenspiel_end
    */
    ```
    If this clause is missing, the build will fail.

6. Upload

   ```bash
   cmake --build . --target upload
   ```

  If the upload did not succeed, it is likely that your keyboard was not recognized by the build system. If this
  happened, don't panic. Just unplug and replug the keyboard, then repeat steps 4 and 6.
