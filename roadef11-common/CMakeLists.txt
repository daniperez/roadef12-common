# Library sources. All listed here for simplicity, but tailored
# compilations can be done in a given subdir if a CMakeList is
# provided there and add_subdirectory is used here (and the
# corresponding sources removed from here, of course).

file ( GLOB_RECURSE lib_SOURCES "*.cpp" )

# Configuration 
configure_file ( config.h.in config.h @ONLY )

# Assembling
add_library ( ${PROJECT_NAME}lib SHARED ${lib_SOURCES} )
add_executable ( ${PROJECT_NAME} ExampleMain.cpp )
target_link_libraries ( "${PROJECT_NAME}lib" ${Boost_LIBRARIES} ${GLOG_LIBRARY} checkerlib )
##
target_link_libraries ( "${PROJECT_NAME}"    ${PROJECT_NAME}lib )
set_target_properties ( "${PROJECT_NAME}lib"  PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" )
set_target_properties ( "${PROJECT_NAME}"     PROPERTIES COMPILE_FLAGS "-std=c++0x -Wall -Werror" )
set_target_properties ( "${PROJECT_NAME}lib"  PROPERTIES COMPILE_FLAGS "-std=c++0x -Wall -Werror" )
