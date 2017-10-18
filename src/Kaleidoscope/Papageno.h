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
