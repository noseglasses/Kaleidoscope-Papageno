#  -*- mode: cmake -*-
# Kaleidoscope-Papageno -- Papageno features for Kaleidoscope
# Copyright (C) 2017 noseglasses <shinynoseglasses@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(papageno_library_directory "${CMAKE_SOURCE_DIR}/../papageno")

if(NOT EXISTS "${papageno_library_directory}")
   
   file(MAKE_DIRECTORY "${papageno_library_directory}")

   # Configure build of a Papageno Arduino library.
   #
   execute_process(
      COMMAND "${CMAKE_COMMAND}" 
         "-DPAPAGENO_ARDUINO_BUILD_DIR=${papageno_library_directory}"
         "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/3rd_party/Papageno/cmake/toolchains/Toolchain-avr-gcc.cmake"
         "-DPAPAGENO_PLATFORM=atmega32u4"
         "${CMAKE_SOURCE_DIR}/3rd_party/Papageno"
      WORKING_DIRECTORY "${papageno_library_directory}"
   )
   
   # Build a Papageno Arduino library.
   #
   execute_process(
      COMMAND "${CMAKE_COMMAND}" --build "${papageno_library_directory}" --target arduino_symlinks
   )
   
endif()