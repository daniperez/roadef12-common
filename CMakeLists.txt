cmake_minimum_required ( VERSION 2.6 )

# -------------------------------------------------------------------
# MAIN 
# -------------------------------------------------------------------
project ( roadef11-common )
set ( roadef11_VERSION_MAJOR 0 )
set ( roadef11_VERSION_MINOR 1 )
set ( roadef11_VERSION_PATCH 0 )

# -------------------------------------------------------------------
# PACKAGING
# -------------------------------------------------------------------
include ( InstallRequiredSystemLibraries )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY
      "roadef11 common headers (parser, objects, ...)" )
set ( CPACK_PACKAGE_VENDOR "daniperez" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${roadef11_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${roadef11_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${roadef11_VERSION_PATCH}" )
set ( CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING" )
set ( CPACK_PACKAGE_FILE_NAME
      "${PROJECT_NAME}-${roadef11_VERSION_MAJOR}-${roadef11_VERSION_MINOR}-${roadef11_VERSION_PATCH}" )
set ( CPACK_RPM_PACKAGE_SUMMARY "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}" )
set ( CPACK_RPM_PACKAGE_DESCRIPTION
      "Google's ROADEF Challenge (2011-2012) starter kit with a parser and 
      handy classes to solve the problem. 'roadef11-common' is a header-only 
      library. You can find an example of use in example/ExampleMain.cpp. The 
      only dependencies of the library are the STL and Boost's shared_ptr and 
      foreach. The author is neither affiliated nor endorsed by Google or 
      Roadef." )
set ( CPACK_RPM_PACKAGE_LICENSE "MIT" )
set ( CPACK_RPM_PACKAGE_VENDOR "Dani Perez" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Libraries" )
set ( CPACK_RPM_PACKAGE_REQUIRES "boost >= 1.40.0, boost-devel >= 1.40.0" )
set ( CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog" )
set ( CPACK_GENERATOR "ZIP;RPM" )

install( DIRECTORY roadef11-common
         DESTINATION include/
         FILES_MATCHING PATTERN *.hpp
       )
install( DIRECTORY roadef11-material/
         DESTINATION share/${PROJECT_NAME}/material
         FILES_MATCHING PATTERN *.txt
       )

install( FILES scripts/FindRoadef11Common.cmake
         DESTINATION share/${PROJECT_NAME}/cmake/ )

install( FILES example/ExampleMain.cpp 
         DESTINATION share/${PROJECT_NAME}/example/ )

install( FILES README.md COPYING ChangeLog
         DESTINATION share/${PROJECT_NAME}/ )

# -------------------------------------------------------------------
# DEPENDENCIES 
# -------------------------------------------------------------------
# CMake scripts path
  set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/ )

# * BOOST
  find_package ( Boost 1.44.0
                 COMPONENTS program_options date_time unit_test_framework )
  if ( Boost_FOUND)
      include_directories ( ${Boost_INCLUDE_DIRS} )
  endif ()

# * DOXYGEN
  set ( DOXYFILE_LATEX "NO" )
  set ( DOXYFILE_SOURCE_DIR "${PROJECT_NAME}" )
  set ( DOXYFILE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc" )
  include ( UseDoxygen-1.2 OPTIONAL )
  install ( DIRECTORY ${DOXYFILE_OUTPUT_DIR}/
            DESTINATION share/${PROJECT_NAME}/doc OPTIONAL )

# * The only purpose of this find_package is to load a pair of macros we
#   put in FindRoadef11Common.cmake
find_package ( Roadef11Common )

# -------------------------------------------------------------------
# HEADERS 
# -------------------------------------------------------------------
include_directories ( BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

# -------------------------------------------------------------------
# SUBDIRS
# -------------------------------------------------------------------
add_subdirectory ( ${PROJECT_NAME} )

# -------------------------------------------------------------------
# TARGET : checker
# -------------------------------------------------------------------
file ( GLOB_RECURSE
       checker_SOURCES
       "roadef11-material/solution_checker/solution_checker.cc" )
add_library ( checkerlib ${checker_SOURCES} )
add_executable ( checker
                 "roadef11-material/solution_checker/solution_checker_run.cc" )
target_link_libraries ( checker checkerlib ) 

# -------------------------------------------------------------------
# TARGET : ExampleMain 
# -------------------------------------------------------------------
add_executable ( ExampleMain example/ExampleMain.cpp )
target_link_libraries ( ExampleMain ${Boost_LIBRARIES} )
set_target_properties ( ExampleMain
                        PROPERTIES COMPILE_FLAGS
                        "-std=c++0x -Wall -Werror" )

# -------------------------------------------------------------------
# TARGET : tests 
# -------------------------------------------------------------------
if ( Boost_FOUND )

    include ( add_test_suite )

    set ( DATA_DIR "${CMAKE_SOURCE_DIR}/roadef11-material/data/" )
    enable_testing() 

    # i) Main Test Suite
    set (
            MainTestSuiteSources 
            tests/MainTest.cpp
            tests/sanity/SanityTest.cpp
        )

    add_test_suite ( MainTestSuite "${MainTestSuiteSources}" )

    target_link_libraries ( MainTestSuite ${Boost_LIBRARIES} )

    # ii) Example instances
    solve_instance (
      "$<TARGET_FILE:ExampleMain>"
      "$<TARGET_FILE:checker>"
      "example"
      "${DATA_DIR}/data_example/model_example.txt"
      "${DATA_DIR}/data_example/initial_solution_example.txt"
      "${DATA_DIR}/data_example/initial_solution_example.txt"
      "-u" )

    # iii) A1 instances
    foreach ( i RANGE 1 5 )
      solve_instance (
        "$<TARGET_FILE:ExampleMain>"
        "$<TARGET_FILE:checker>"
        "a1-${i}"
        "${DATA_DIR}/data_a1/model_a1_${i}.txt"
        "${DATA_DIR}/data_a1/assignment_a1_${i}.txt"
        "${CMAKE_BINARY_DIR}/solution_a1_${i}.txt"
        "" )
    endforeach ()

    # iv) Doxgygen run (just seeking for missing documentation)
    add_doxygen_test_suite ()

    # Don't forget to add the test suite to the DEPENDS argument list.
    # a1-1,a1-2,... targets write log messages whereas "check" doesn't
    # (only in case of failure)
    add_custom_target(
                      check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure 
                      DEPENDS ExampleMain MainTestSuite checker
                     )
endif ()

# Keep this last.
include ( CPack )

