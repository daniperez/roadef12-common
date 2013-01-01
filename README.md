Roadef12-Common
===============

[![Build Status](https://travis-ci.org/daniperez/roadef12-common.png)](https://travis-ci.org/daniperez/roadef12-common.txt)

About
-----

This is a starter kit for Google's ROADEF Challenge (2011-2012), with a parser
and handy classes to solve the problem. roadef12-common is a header-only
library. You can find an example of use in example/ExampleMain.cpp. The only
dependencies of the library are the STL and Boost's shared_ptr and foreach.

The author is neither affiliated nor endorsed by Google or ROADEF.

More info about the challenge can be found [here](http://challenge.roadef.org/2012/en/index.php).

Installation 
------------

roadef12-common is a header-only library and thus putting those headers
in any location reachable by your compiler will do the trick.

A more involving installation process requires the use of CMake:

1. Install the tarball or RPM
2. Create a CMakeLists.txt like the one found in examples/CMakeLists.txt.
   In summary:
    1. Find the package: find_package ( roadef12-common )
    2. If the package is found, it will define:
        * roadef12-common_INCLUDE_DIRS: where the headers were installed.  
        * roadef12-common_DATA: folder containing the data instances.
        * roadef12-common_FOUND: true if the library was found.
        * Macro 'solve_all_instances' 
    3. Compile with roadef12-common_INCLUDE_DIRS in your include path and
       use solve_all_instances to check your code agains all the instances
       (doc in scripts/roadef12-common-config.cmake.in).


How to use it 
-------------

roadef12-common is a header-only library. You can find an example
of use in example/ExampleMain.cpp.

In summary, the steps shown in ExampleMain.cpp are:

1.  Subclass Service class (roadef12-common/service/Service.hpp).
    Only Service::optimize and constructor have to be redefined.
2.  Call your Service subclass with the input given by 
    ProgramOptions class (roadef12-common/service/ProgramOptions.hpp).
3.  Service class has the "params" attribute with the problem
    parameters. "options" contains the command line options passed
    to the executable in ProgramOptions. "firstAssignment" contains
    the initial assignment of processes to machines.

The only dependencies of the library are the STL and Boost's shared_ptr,
lexical_cast and foreach.
