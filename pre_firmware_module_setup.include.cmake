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
   
# message("Module source dir: ${KALEIDOSCOPE_MODULE_SOURCE_DIR}")
message("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message("KALEIDOSCOPE_HOST_BUILD: ${KALEIDOSCOPE_HOST_BUILD}")

# Setup papageno as a Arduino library
#
message("Start setup papageno")
execute_process(
   COMMAND "${CMAKE_COMMAND}"
      "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
      "-DKALEIDOSCOPE_MODULE_SOURCE_DIR=${KALEIDOSCOPE_MODULE_SOURCE_DIR}"
      "-DKALEIDOSCOPE_HOST_BUILD=${KALEIDOSCOPE_HOST_BUILD}"
       -P "${KALEIDOSCOPE_MODULE_SOURCE_DIR}/setup_library.script.cmake"
   WORKING_DIRECTORY "${KALEIDSCOPE_MODULE_SOURCE_DIR}"
)
message("End setup papageno")

# Make sure that any papageno headers are found during the firmware build.
#
include_directories("${KALEIDOSCOPE_MODULE_SOURCE_DIR}/../papageno/src")

# Configure and build Papageno with Glockenspiel
#
set(papageno_build_dir "${KALEIDOSCOPE_MODULE_BINARY_DIR}/papageno")

file(MAKE_DIRECTORY "${papageno_build_dir}")

# The variable KALEIDOSCOPE_MODULE_SOURCE_DIR is defined by the 
# including scope

set(cmake_cache_file "${papageno_build_dir}/CMakeCache.txt")
add_custom_command(
   OUTPUT "${cmake_cache_file}"
   COMMAND "${CMAKE_COMMAND}" 
      "-DPAPAGENO_BUILD_LIBRARY=FALSE"
      "-DPAPAGENO_BUILD_GLOCKENSPIEL=TRUE"
      "-DPAPAGENO_TESTING_ENABLED=FALSE"
      "${KALEIDOSCOPE_MODULE_SOURCE_DIR}/3rd_party/Papageno"
   WORKING_DIRECTORY "${papageno_build_dir}"
)

add_custom_target(kaleidoscope_papageno_configure DEPENDS "${cmake_cache_file}")

if(CMAKE_HOST_WIN32)
   set(glockenspiel_executable "${papageno_build_dir}/glockenspiel/glockenspiel.exe")
else()
   set(glockenspiel_executable "${papageno_build_dir}/glockenspiel/glockenspiel")
endif()

add_custom_command(
   OUTPUT "${glockenspiel_executable}"
   COMMAND "${CMAKE_COMMAND}"
      --build .
   WORKING_DIRECTORY "${papageno_build_dir}"
)

add_custom_target(kaleidoscope_papageno_build DEPENDS "${glockenspiel_executable}")
add_dependencies(kaleidoscope_papageno_build kaleidoscope_papageno_configure)

# Prepare the Papageno compile of the sketch
#
get_filename_component(sketch_path "${KALEIDOSCOPE_FIRMWARE_SKETCH}" PATH)

set(kaleidoscope_papageno_source "${sketch_path}/Kaleidoscope-Papageno-Sketch.hpp")

message("KALEIDOSCOPE_FIRMWARE_SKETCH=${KALEIDOSCOPE_FIRMWARE_SKETCH}")
add_custom_command(
   OUTPUT "${kaleidoscope_papageno_source}"
   DEPENDS "${KALEIDOSCOPE_FIRMWARE_SKETCH}"
   DEPENDS "${glockenspiel_executable}"
   COMMAND "${glockenspiel_executable}" 
      -I "${sketch_path}" 
      -i "${KALEIDOSCOPE_MODULE_SOURCE_DIR}/glockenspiel/predefines.gls"
      -i "${KALEIDOSCOPE_FIRMWARE_SKETCH}"
      -o "${kaleidoscope_papageno_source}"
      -p "Kaleidoscope/Papageno.h"
   WORKING_DIRECTORY "${KALEIDOSCOPE_MODULE_SOURCE_DIR}"
)

add_custom_target(kaleidoscope_papageno_glockenspiel_compile DEPENDS "${kaleidoscope_papageno_source}")
add_dependencies(kaleidoscope_papageno_glockenspiel_compile kaleidoscope_papageno_build)

list(APPEND modules_additional_headers "${kaleidoscope_papageno_source}")
