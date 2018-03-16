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

# Configure and build Papageno with Glockenspiel
#
set(papageno_build_dir "${CMAKE_BINARY_DIR}/glockenspiel")

file(MAKE_DIRECTORY "${papageno_build_dir}")

execute_process(
   COMMAND "${CMAKE_COMMAND}" 
      "-DPAPAGENO_BUILD_GLOCKENSPIEL=TRUE"
      "${CMAKE_SOURCE_DIR}/3rd_party/Papageno"
   WORKING_DIRECTORY "${papageno_build_dir}"
)

execute_process(
   COMMAND "${CMAKE_COMMAND}"
      --build .
   WORKING_DIRECTORY "${papageno_build_dir}"
)

# Prepare the Papageno compile of the sketch
#
get_filename_component(sketch_path PATH "${KALEIDOSCOPE_FIRMWARE_SKETCH}")

# TODO: Make this portable!
find_program(
   GLOCKENSPIEL_EXECUTABLE
   NAMES glockenspiel glockenspiel.exe
   PATHS "${papageno_build_dir}/glockenspiel"
)

if("${GLOCKENSPIEL_EXECUTABLE}" STREQUAL "GLOCKENSPIEL_EXECUTABLE-NOTFOUND")
   message(FATAL_ERROR "Unable to find GLOCKENSPIEL_EXECUTABLE")
endif()

set(kaleidoscope_papageno_source "${sketch_path}/Kaleidoscope-Papageno.cpp")

execute_process(
   COMMAND "${GLOCKENSPIEL_EXECUTABLE}" 
      -d
      -I "${sketch_path}" 
      -i "${KALEIDOSCOPE_FIRMWARE_SKETCH}"
      -o "${kaleidoscope_papageno_source}"
      -p "Papageno.h"
   OUTPUT_VARIABLE output
   ERROR_VARIABLE error
   RESULT_VARIABLE result
)

if(NOT "${RESULT_VARIABLE}" EQUAL 0)
   message("output: ${output}")
   message("error: ${error}")
   message("result: ${result}")
   message(FATAL_ERROR "Glockenspiel build of firmware sketch ${KALEIDOSCOPE_FIRMWARE_SKETCH} failed")
endif()

message("Created Kaleidoscope-Papageno source file \"${kaleidoscope_papageno_source}\"")
message("Important: Include this file at the very bottom of your firmware sketch!")
