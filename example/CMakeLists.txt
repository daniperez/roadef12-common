cmake_minimum_required ( VERSION 2.6 )

# This points to the location of Roadef12 macros: 
# TODO: is this needed anymore?
set ( Roadef12CommonMacroLocation "${CMAKE_SOURCE_DIR}/scripts/" )
set ( CMAKE_MODULE_PATH ${Roadef12CommonMacroLocation} )

# -------------------------------------------------------------------
# DEPENDENCIES 
# -------------------------------------------------------------------
# CMAKE_BINARY_DIR used here to allow compiling the example within
# roadef12-common source tree.
set ( roadef12-common_DIR ${CMAKE_BINARY_DIR} )
find_package ( roadef12-common )

# -------------------------------------------------------------------
# HEADERS 
# -------------------------------------------------------------------
include_directories ( BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
                             ${roadef12-common_INCLUDE_DIRS} )

# -------------------------------------------------------------------
# TARGET : Main 
# -------------------------------------------------------------------
add_executable ( Main ExampleMain.cpp )
target_link_libraries ( Main ${Boost_LIBRARIES} )
set_target_properties ( Main
                        PROPERTIES COMPILE_FLAGS
                        "-std=c++0x -Wall -Werror" )

# -------------------------------------------------------------------
# TARGET : check 
# -------------------------------------------------------------------
solve_all_instances ( "Main" "${roadef12-common_DATA}" )

# Don't forget to add the test suite to the DEPENDS argument list.
# a1-1,a1-2,... targets write log messages whereas "check" doesn't
# (only in case of failure)
add_custom_target(
                  check-instances 
                  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure 
                  DEPENDS Main roadef12-checker
                 )

