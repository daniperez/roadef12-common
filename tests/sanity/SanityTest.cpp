// The MIT License
// 
// Copyright (c) 2011 daniperez
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/test/unit_test.hpp>
///////////////////////////////////////////////////////////////////////////
// roadef11
#include "roadef11-common/service/Service.hpp"
#include "roadef11-common/service/ProgramOptions.hpp"
#include "roadef11-common/config.h"
///////////////////////////////////////////////////////////////////////////

ROADEF11::ServiceInput getExampleServiceInput()
{
    ROADEF11::ServiceInput input;
    
    input.parameters = std::string(PROJECT_SOURCE_DIR) + "/roadef11-material/data/data_example/model_example.txt";
    input.referenceSolution = std::string(PROJECT_SOURCE_DIR) + "/roadef11-material/data/data_example/initial_solution_example.txt";
    input.solution = std::string(PROJECT_SOURCE_DIR) + "/roadef11-material/data/data_example/new_solution_example.txt";
    
    return input;
}

BOOST_AUTO_TEST_SUITE( MainTestSuite )

BOOST_AUTO_TEST_CASE( SanityCheck01 )
{
    BOOST_CHECK( 1 == (2/2) );
}

BOOST_AUTO_TEST_CASE( SanityCheck02 )
{
    const char* commandLine[] = { "foo", "-name" };
    
    // This will print the team's name in the stdout and
    // throw DoNotContinue exception.
    BOOST_CHECK_THROW (
        ROADEF11::ProgramOptions::parseCommandLine ( 2, const_cast<char**>(commandLine)),
        ROADEF11::DoNotContinue
    );
}

#ifdef USE_CHECKER
BOOST_AUTO_TEST_CASE( CheckerCheck )
{
    ROADEF11::ServiceInput input = getExampleServiceInput();
   
    ROADEF11::Service service ( input );
    
    BOOST_CHECK ( service.check() == 2411 );
}
#endif

BOOST_AUTO_TEST_SUITE_END()


