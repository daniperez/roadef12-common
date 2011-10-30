cmake_minimum_required ( VERSION 2.6 )

# -------------------------------------------------------------------
# MAIN 
# -------------------------------------------------------------------
project ( roadef11-common )
set ( roadef11_VERSION_MAJOR 0 )
set ( roadef11_VERSION_MINOR 2 )
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
       "roadef11-material/solution_checker/solution_checker.cc" )
add_library ( checkerlib ${checker_SOURCES} )
add_executable ( roadef11-checker
                 "roadef11-material/solution_checker/solution_checker_run.cc" )
target_link_libraries ( roadef11-checker checkerlib )

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

    # ii) Doxgygen run (just seeking for missing documentation)
    add_test (
               NAME "DoxygenErrors" 
               WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
               COMMAND "${CMAKE_SOURCE_DIR}/scripts/doxytest.sh"
                       "${DOXYGEN_EXECUTABLE}"
                       "${DOXYFILE}"
             )

endif ()

# -------------------------------------------------------------------
# INSTALLATIONS
# -------------------------------------------------------------------

# Headers
install ( DIRECTORY roadef11-common
          DESTINATION include
          FILES_MATCHING PATTERN *.hpp )

# Material
install( DIRECTORY roadef11-material/
         DESTINATION share/${PROJECT_NAME}/material
         FILES_MATCHING PATTERN *.txt
       )

# Example
install( FILES example/ExampleMain.cpp  example/CMakeLists.txt
         DESTINATION share/${PROJECT_NAME}/example/ )

# CMake scripts
configure_file (
  "scripts/Roadef11CommonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Roadef11CommonConfig.cmake" @ONLY
)
install ( FILES "${CMAKE_CURRENT_BINARY_DIR}/Roadef11CommonConfig.cmake"
                "scripts/add_test_suite.cmake"
                DESTINATION share/${PROJECT_NAME}/cmake/ )

# Misc
install( FILES README.md COPYING ChangeLog
         DESTINATION share/${PROJECT_NAME}/ )

# Main target: checker
# It's exported by means of CMake's export interface (i.e.
# we can easily import the target in other projects).
install ( TARGETS roadef11-checker
          EXPORT        Roadef11Checker 
          RUNTIME       DESTINATION bin
          PUBLIC_HEADER DESTINATION include
          LIBRARY       DESTINATION lib
        )

install ( EXPORT Roadef11Checker DESTINATION share/${PROJECT_NAME}/cmake/ )

# -------------------------------------------------------------------
# SUBDIRS
# -------------------------------------------------------------------
add_subdirectory ( ${PROJECT_NAME} )
add_subdirectory ( example )

include ( CPack )

