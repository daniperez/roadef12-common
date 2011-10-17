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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE example
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/test/unit_test.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
///////////////////////////////////////////////////////////////////////////

// This can be set in runtime:
//
// e.g.: ./roadef11/TestSuite --output_format=XML --report_level=detailed \
//       --show_progress=yes
//
struct MyConfig
{
    MyConfig()
    {
        boost::unit_test::unit_test_log.set_threshold_level
        (
            boost::unit_test::log_successful_tests
        ); 

        boost::unit_test::unit_test_log.set_format( boost::unit_test::CLF );
    }

    ~MyConfig()
    {
    }
};

BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_SUITE( MainTestSuite )

// Tests cases are under test/* folders but 
// they could have been also defined here.
//
// Suites of tests can be used to separate test
// nature: long-run tests, unit tests, integration
// tests...

BOOST_AUTO_TEST_SUITE_END()

