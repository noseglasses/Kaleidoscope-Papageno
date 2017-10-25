<!--![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-TapDance.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-TapDance

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52-->

# Kaleidoscope-Papageno: Turn your keyboard into a magical instrument

## Abstract

Define multi-key tap-dances, highly customizable leader key-sequences, chords, key-clusters or mixtures of it all. [Papageno](https://github.com/noseglasses/papageno/) an advanced pattern matching library seamlessly integrates with Kaleidoscope to provide ultimate flexibility in the generation of multi-key-commands. An automatized compression mechanism converts easy-to-define search-tree data structures into statically allocated, compile-time static compressed data-sets. This allows for Flash and RAM memory-saving definitions of large amounts of key-patterns in a clearly arranged fashion.

## Introduction

A description of Papageno, its basic features and a motivation of its development and application is followed by a brief example how to use the Papageno-Kaleidoscope API in `key-map.c` definitions. Links to further documents that describe Papageno and its Kaleidoscope-API are provided at the end of the document.

## Motivation

### Utilization of thumb keys on ergonomic keyboards

Most modern ergonomic keyboards come with dedicated thumb key clusters. These are mainly motivated by the obvious under-utilization of the thumbs on traditional computer keyboards. 
As our thumbs are the most powerful fingers, it is desirable to shift as much work as possible from the other fingers towards the thumbs. The actual number of (reachable) thumb keys varies between different keyboard designs. Typically there are between four and eight (reachable) and freely programmable thumb keys available.
This restriction in number means that when assigning key-codes or actions to these special keys it is important to do it in a way that optimizes both, utilization and usability. In the recent past there occurred a significant number of innovative ideas that aim to assign more than two key-codes or actions to a physical key.

### Existing Kaleidoscope-features

Tap-dances, modifier keys, with and without one-touch, as well as multiple key-map-layers provide different ways to creatively assign work to thumb keys using Kaleidoscope. These methods all have specific advantages and drawbacks. A thorough analysis of the many possible ways to utilize and combine those could fill a research article of its own. Therefore, we will concentrate on tap-dances to explain the enhancements introduced by Papageno. 

Tap-dances theoretically allow to assign an infinite number of meanings to a single physical key. A specific number of key-presses, usually entered in rapid succession, triggers a specific action.
This is probably the only, but also the greatest disadvantage of *traditional* tap-dances. Hitting a key a given number of times in a row can be fairly difficult to accomplish, especially for non pianists and when there are definitions for say, three, four and five strokes of the same key.

### Towards multi-key tap-dances

As a keyboard comes with a multitude of keys, why not extend the concept of tap-dances to combinations of several keys? It is much easier to hit two, three or more keys in a defined order than to hit the same key a given number of times. Entering key sequences, i.e. words, is precisely what typists are trained to do.
 
Of course, we want the action that is triggered when the key sequence has been completed to replace the actual series of key-codes that is assigned with the keys when hit independently, same as with tap-dances.

### Some numbers

Let's go back to where we came from, shifting work, e.g. to thumb keys. It would be useful to assign actions to two key combinations (two key words) of thumb keys that are assigned to the left and the right hand.
For a keyboard with two thumb keys for each hand, this would mean that we would end up with eight possible two-key combinations of thumb keys assigned to different hands. Hereby, we assume that hitting two keys A and B in two different orders is supposed to trigger two different actions. 

If we add to these eight combinations the four possible actions assigned to keys when hit independently, this sums up to twelve different actions for four thumb keys.

It is up to the reader to solve the task of computing the drastic increase in number if we would also consider three-key combinations and so forth.

### A real-life use case

Let's look at a brief example how multi-key tap-dances (in Papageno jargon called single-note lines, clusters and chords) could be applied to enhance Kaleidoscope's capability. 

Imagine editing some sort of program source code. A hereby common task is to indent lines, either to improve readability or because the language requires indentation as part of its semantic. Often it is also necessary to un-indent lines of code. Most editors assign shortcuts to both operations that are more or less simple to enter.
As indentation/un-indentation are a fairly common operations when programming, it seems to be a good idea to trigger them through thumb-keys of our possibly ergonomic, programmable keyboard. 

In the following we use the letter `A` for one of the left hand thumb-keys and `B` for one of the right hand thumb keys. 
One possible solution for the given task would be to indent
 when keys `A` and `B` are hit in order `A-B` and un-indent when hit in order `B-A`. Both operations would, of course, be programmed to emit the required shortcut.
 
From a neuromuscular point of view this key-assignment strategy is doubtlessly a good choice as it is fairly easy to learn. Moreover, it is rather intuitive as we assign a forward-backward relation to indentation and un-indentation with respect to the order of the two thumb keys being hit. This certainly aids memorizing our new key assignment.

### Papageno - a pattern matching library

To bring the above example to life, we need a mechanism that is capable to recognize more or less complex keystroke patterns. The requirements described, among others,
led to the development of [Papageno](https://github.com/noseglasses/papageno/), an advanced pattern matching library. Originally implemented as part of Kaleidoscope, Papageno emerged to a stand-alone multi-platform library that still seamlessly integrates with Kaleidoscope. It provides a variety of different ways to define patterns that go far beyond advanced tap-dances only.

<!--## A Kaleidoscope example

The impatient may directly jump to a [Kaleidoscope example key-map](https://github.com/noseglasses/noseglasses_qmk_layout/) that demonstrates how Papageno can be employed to effectively shift load from fingers to thumbs.-->

## Papageno as a Kaleidoscope plugin

Papageno allows for the definition of general multi-key patterns, chords, key clusters, arbitrary leader sequences with multiple leader keys and more. All features utilize the same efficient and optimized pattern matching algorithm. To be used with Kaleidoscope, Papageno provides a wrapper API. It allows to define patterns in a simple and readable way and encapsules common tasks through C-pre-processor macros.

The heart of Papageno is a search tree that is established based on the definitions that are part of user-implemented Kaleidoscope sketches.
As typical for dynamically assembled tree data structures, this tree-based approach relies on dynamic memory allocation. Many believe that dynamically allocated data structures are a no-go on embedded architectures, such as those used in programmable keyboards. Actually, it is not that much of a hindrance as Papageno mainly allocates memory but does not free it during program execution. This inherently avoids RAM fragmentation which would otherwise lead to an early exhaustion of RAM. 

To optimize the utilization of both, Flash memory and SRAM, Papageno comes with an integrated compression mechanism.

## Compression of data-structures

Where the dynamic creation of tree data structures works well for a moderate amount of key patterns, it can be problematic when a large amount of patterns is defined or if many other features of Kaleidoscope are used along with Papageno. In such a case the limited resources of the keyboard hardware might be a problem.

Before we talk about compression of data structures, let us look at the two modes of operation, that Papageno provides. The first and general one requires the generation of dynamic data structures during firmware execution on the keyboard. The second, a more advanced and optimized mode of operation allows for a two stage compile process that compresses most of Papageno's data structures and aims to be as memory efficient as possible, both with respect to flash memory and RAM.

This integrated compression mechanism turns the dynamically allocated pattern matching search tree into a compile-time static data structure. We, thus, avoid dynamic memory allocations and significantly shrink binary size. Apart from the sheer compactification of data structures, compression also helps to safe program memory as no code is required to establish dynamic data structures during firmware execution. The respective C-functions are, therefore, automatically stripped from the binary as part of the firmware build process. 

When applied to [noseglasses'](https://github.com/noseglasses/noseglasses_qmk_layout/) QMK-key-map compiled for an ErgoDox EZ, Papageno's compression mechanism reduces the size of the emerging hex-file by 2420 byte. The same numbers can be expected when using Papageno with Kaleidoscope. Program storage and RAM saved can e.g. be used for other valuable Kaleidoscope features.

## Compatibility with existing Kaleidoscope features

The efficiency of Papageno's tree based pattern matching approach is one of the reasons why some features that were already part of Kaleidoscope, such as tap-dances and leader sequences, have been incorporated into Papageno. (Another reason is that they were just fun to implement based on Papageno).

Let's concentrate on our primary motivation, efficiency.
If features provided by Papageno are used in common with equivalent pre-existing Kaleidoscope-features, such as e.g. leader sequences, that are now also supported by Papageno, resource usage can be expected to be significantly higher than if Papageno would deal with the given tasks alone. The reason is obvious, more program memory and probably more RAM is required to store binary code and variables for two or more modules instead of only one. 

Although optimized resource utilization is never a bad idea, Papageno plays along well with other Kaleidoscope features.

## Papageno as a wrapper to Kaleidoscope

Papageno-Kaleidoscope is designed as a wrapper for the rest of Kaleidoscope. Any keystrokes are intercepted and passed through Papageno's pattern matching engine. Only if a series of keystrokes is identified as non-matching any defined pattern, it is passed over to the main Kaleidoscope engine and to other plugins. To Kaleidoscope these keystrokes appear as if they had just emerged during the most recent keyboard matrix scan.

This allows for a strong decoupling of Papageno and Kaleidoscope, which is robust and can be expected to cooperate quite well with most other Kaleidoscope plugins.

For compatibility reasons, Papageno provides a layer mechanism that is fairly similar to that of Kaleidoscope's key-maps. At their point of definition, patterns are associated with layers. They are only active while their associated layer is the currently highest Kaleidoscope layer.
Layer fall through, works similar to the assignment of transparent key-codes in Kaleidoscope key-maps.

It is most common to emit Kaleidoscope key-codes when a defined pattern matches a series of keystrokes.
To allow for more advanced functionality Papageno provides an interface to define arbitrary user callback functions that can be supplied with user defined data.