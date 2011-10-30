cmake_minimum_required ( VERSION 2.6 )

# This points to the location of Roadef11 macros: 
set ( Roadef11CommonMacroLocation "${CMAKE_SOURCE_DIR}/scripts/" )
set ( CMAKE_MODULE_PATH ${Roadef11CommonMacroLocation} )

# -------------------------------------------------------------------
# DEPENDENCIES 
# -------------------------------------------------------------------
# CMAKE_BINARY_DIR used here to allow compiling the example within
# roadef11-common source tree.
set ( roadef11-common_DIR ${CMAKE_BINARY_DIR} )
find_package ( roadef11-common )

# -------------------------------------------------------------------
# HEADERS 
# -------------------------------------------------------------------
include_directories ( BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
                             ${roadef11-common_INCLUDE_DIRS} )

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
solve_all_instances ( "Main" "${roadef11-common_DATA}" )

# Don't forget to add the test suite to the DEPENDS argument list.
# a1-1,a1-2,... targets write log messages whereas "check" doesn't
# (only in case of failure)
add_custom_target(
                  check-instances 
                  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure 
                  DEPENDS Main roadef11-checker
                 )

