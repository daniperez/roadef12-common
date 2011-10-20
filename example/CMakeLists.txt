cmake_minimum_required ( VERSION 2.6 )

# Set this to the location of FindRoadef11Common.cmake
set ( Roadef11CommonFinderLocation "${PROJECT_SOURCE_DIR}/scripts/" )

# -------------------------------------------------------------------
# DEPENDENCIES 
# -------------------------------------------------------------------
# CMake scripts path
set ( CMAKE_MODULE_PATH ${Roadef11CommonFinderLocation} )

# * ROADEF11-Common
find_package ( Roadef11Common )

# -------------------------------------------------------------------
# HEADERS 
# -------------------------------------------------------------------
include_directories ( BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

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
# i) Example instance
solve_instance (
  "Main"
  "example" # You can do 'make example' to solve the example instance
  "${DATA_DIR}/data_example/model_example.txt"
  "${DATA_DIR}/data_example/initial_solution_example.txt"
  "${DATA_DIR}/data_example/initial_solution_example.txt"
  "-u" )

# ii) A1 instances
foreach ( i RANGE 1 5 )
  solve_instance (
    "Main"
    "a1-${i}" # You can do 'make a1-[1-4]' to solve a single A1 instance 
    "${DATA_DIR}/data_a1/model_a1_${i}.txt"
    "${DATA_DIR}/data_a1/assignment_a1_${i}.txt"
    "${CMAKE_BINARY_DIR}/solution_a1_${i}.txt"
    "" )
endforeach ()

# Don't forget to add the test suite to the DEPENDS argument list.
# a1-1,a1-2,... targets write log messages whereas "check" doesn't
# (only in case of failure)
add_custom_target(
                  check
                  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure 
                  DEPENDS Main ROADEF11COMMON_checker
                 )

