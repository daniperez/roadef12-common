# The MIT License
#
# Copyright (c) 2011 daniperez
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
############################################################################
# parse_arguments.cmake belongs to Cmake authors. See
#
#   http://www.cmake.org/Wiki/CMakeMacroParseArguments
#
include ( parse_arguments )
############################################################################
 
#
# init_autotest_cpp - Sets up the environment to use autotest-cpp. Checks
# if dependencies are installed. 
#
#  init_autotest_cpp ( [REQUIRED] exclude_folder ) 
#
# REQUIRED makes the macro to abort the execution if autotest-cpp cannot be
# properly set up.
#
macro ( init_autotest_cpp )

  parse_arguments ( "AUTOTEST" "INCLUDE;EXCLUDE" "REQUIRED" ${ARGN} )

  set ( EXCLUDE_FOLDER ${AUTOTEST_DEFAULT_ARGS} CACHE INTERNAL "Exclude this folder" )

  find_program ( INOTIFYWAIT inotifywait )

  find_program ( NOTIFY-SEND notify-send )

  if ( INOTIFYWAIT STREQUAL "INOTIFYWAIT-NOTFOUND" AND AUTOTEST_REQUIRED )

    message ( SEND_ERROR "autotest -- Didn't find REQUIRED inotifywait application. It's part of 'inotify-tools' package." )

  endif ()

  if ( NOTIFY-SEND STREQUAL "NOTIFY-SEND-NOTFOUND" AND AUTOTEST_REQUIRED )

    message ( SEND_ERROR "autotest -- Didn't find REQUIRED notify-send application. It's part of 'libnotify' package." )

  endif ()

  find_program ( AUTOTEST_SCRIPT autotest-cpp.sh PATH ${CMAKE_MODULE_PATH} )

  if ( AUTOTEST_SCRIPT STREQUAL "AUTOTEST_SCRIPT-NOTFOUND" )

    message ( SEND_ERROR "autotest -- cannot find autotest-cpp.sh" )

  endif ()

  file( GLOB_RECURSE INCLUDE_FILES "${AUTOTEST_INCLUDE}/*.cpp" "${AUTOTEST_INCLUDE}/*.hpp" )

  # make autotest
  add_custom_target(
                     autotest
                     inotifywait -mr --exclude "${AUTOTEST_EXCLUDE}.*" --format "%w%f %e" -e modify -e create -e attrib ${INCLUDE_FILES} 
                     COMMENT "Executing autotest"
               )

  message ( STATUS "autotest -- autotest is working" )
  message ( STATUS "autotest --  * INOTIFYWAIT     = ${INOTIFYWAIT}" )
  message ( STATUS "autotest --  * NOTIFY-SEND     = ${NOTIFY-SEND}" )
  message ( STATUS "autotest --  * autotest-cpp.sh = ${AUTOTEST_SCRIPT}" )
  
endmacro ()

