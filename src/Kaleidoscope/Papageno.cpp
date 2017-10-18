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
