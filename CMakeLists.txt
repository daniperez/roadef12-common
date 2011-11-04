cmake_minimum_required ( VERSION 2.6 )

# -------------------------------------------------------------------
# MAIN 
# -------------------------------------------------------------------
project ( roadef12-common )
set ( roadef12_VERSION_MAJOR 0 )
set ( roadef12_VERSION_MINOR 3 )
set ( roadef12_VERSION_PATCH 1 )
set ( roadef12_VERSION_REL   1 )

# This should be done only for RELWITHDEBINFO, but it doesn't work
set ( CMAKE_EXE_LINKER_FLAGS "-s" CACHE INTERNAL "Strip binaries" )

# -------------------------------------------------------------------
# PACKAGING
# -------------------------------------------------------------------
include ( InstallRequiredSystemLibraries )
set ( CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}-devel" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY
      "ROADEF 2012 common headers (parser, objects, ...)" )
set ( CPACK_PACKAGE_VENDOR "daniperez" )
set ( CPACK_RPM_PACKAGE_URL "http://bitbucket.org/daniperez/roadef12-common" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${roadef12_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${roadef12_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${roadef12_VERSION_PATCH}" )
set ( CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING" )
set ( CPACK_RPM_PACKAGE_VERSION
      "${roadef12_VERSION_MAJOR}.${roadef12_VERSION_MINOR}.${roadef12_VERSION_PATCH}" )
set ( CPACK_RPM_PACKAGE_RELEASE
      "${roadef12_VERSION_REL}" )
set ( CPACK_PACKAGE_FILE_NAME
      "${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}" )
set ( CPACK_RPM_PACKAGE_SUMMARY "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}" )
set ( CPACK_RPM_PACKAGE_DESCRIPTION
      "Google's ROADEF Challenge (2011-2012) starter kit with a parser and 
      handy classes to solve the problem. 'roadef12-common' is a header-only 
      library." )
set ( CPACK_RPM_PACKAGE_LICENSE "MIT" )
set ( CPACK_RPM_PACKAGE_VENDOR "Dani Perez" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Libraries" )
set ( CPACK_RPM_PACKAGE_REQUIRES "boost >= 1.40.0" )
set ( CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog" )
set ( CPACK_GENERATOR "ZIP;RPM" )

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

# -------------------------------------------------------------------
# HEADERS 
# -------------------------------------------------------------------
include_directories ( BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

# -------------------------------------------------------------------
# TARGET : checker
# -------------------------------------------------------------------
file ( GLOB_RECURSE
       checker_SOURCES
       "roadef12-material/solution_checker/solution_checker.cc" )
add_library ( checkerlib ${checker_SOURCES} )
add_executable ( roadef12-checker
                 "roadef12-material/solution_checker/solution_checker_run.cc" )
target_link_libraries ( roadef12-checker checkerlib )

# -------------------------------------------------------------------
# TARGET : tests 
# -------------------------------------------------------------------

include ( add_test_suite )

set ( DATA_DIR "${CMAKE_SOURCE_DIR}/roadef12-material/data/" )
enable_testing() 

if ( Boost_FOUND )
# i) Main Test Suite
    set (
            MainTestSuiteSources 
            tests/MainTest.cpp
            tests/sanity/SanityTest.cpp
        )

    add_test_suite ( MainTestSuite "${MainTestSuiteSources}" )

    target_link_libraries ( MainTestSuite ${Boost_LIBRARIES} )
endif ()

# ii) Doxgygen run (just seeking for missing documentation)
add_test (
           NAME "DoxygenErrors" 
#           WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
           COMMAND "${CMAKE_SOURCE_DIR}/scripts/doxytest.sh"
                   "${DOXYGEN_EXECUTABLE}"
                   "${DOXYFILE}"
         )

# -------------------------------------------------------------------
# INSTALLATIONS
# -------------------------------------------------------------------

# Headers
install ( DIRECTORY roadef12-common
          DESTINATION include
          FILES_MATCHING PATTERN *.hpp )

# Material
install( DIRECTORY roadef12-material/
         DESTINATION share/${PROJECT_NAME}/material
         FILES_MATCHING PATTERN *.txt
       )

# Example
install( FILES example/ExampleMain.cpp  example/CMakeLists.txt
         DESTINATION share/${PROJECT_NAME}/example/ )

# CMake scripts
configure_file (
  "scripts/roadef12-common-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/roadef12-common-config.cmake" @ONLY
)
install ( FILES "${CMAKE_CURRENT_BINARY_DIR}/roadef12-common-config.cmake"
                "scripts/add_test_suite.cmake"
                DESTINATION share/cmake/${CPACK_PACKAGE_FILE_NAME}/ )

# Misc
install( FILES README.md COPYING ChangeLog
         DESTINATION share/${PROJECT_NAME}/ )

# Main target: checker
# It's exported by means of CMake's export interface (i.e.
# we can easily import the target in other projects).
install ( TARGETS roadef12-checker
          EXPORT        roadef12-checker 
          RUNTIME       DESTINATION bin
          PUBLIC_HEADER DESTINATION include
          LIBRARY       DESTINATION lib
        )

install ( EXPORT roadef12-checker DESTINATION share/cmake/${CPACK_PACKAGE_FILE_NAME}/ )

# -------------------------------------------------------------------
# SUBDIRS
# -------------------------------------------------------------------
add_subdirectory ( ${PROJECT_NAME} )
add_subdirectory ( example )

# Keep this last

set ( check_DEPS "" )

if (Boost_FOUND)
  set ( check_DEPS Main MainTestSuite roadef12-checker)
else ()
  set ( check_DEPS Main roadef12-checker)
endif()

add_custom_target(
              check
              COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure 
              DEPENDS ${check_DEPS}
             )

include ( CPack )

