How to use roadef11-common
==========================

roadef11-common is a header-only library. You can find an example
of use in example/ExampleMain.cpp.

In summary, the steps shown in ExampleMain.cpp are:

1.  Subclass Service class (roadef11-common/service/Service.hpp).
    Only Service::optimize and constructor have to be redefined.
2.  Call your Service subclass with the input given by 
    ProgramOptions class (roadef11-common/service/ProgramOptions.hpp).
3.  Service class has the "params" attribute with the problem
    parameters. "options" contains the command line options passed
    to the executable in ProgramOptions. "firstAssignment" contains
    the initial assignment of processes to machines.

The only dependencies of the library are the STL and Boost's shared_ptr,
lexical_cast and foreach.

The library can be checked out in your sources or you can use the
packages in http://bitbucket.org/daniperez/roadef11-common/downloads
