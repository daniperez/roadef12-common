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
// roadef11 
#include "roadef11-common/service/Service.hpp"
#include "roadef11-common/service/ProgramOptions.hpp"
#include "roadef11-common/service/ServiceExceptions.hpp"
#include "roadef11-common/util/Util.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////

int main ( int argc, char **argv )
{
    try
    {
        ROADEF11::ServiceInput input
            = ROADEF11::ProgramOptions::parseCommandLine ( argc, argv );
            
        ROADEF11::Service service ( input );

        std::cout << "Optimization started..." << std::endl;

        service.optimize();
        
        std::cout << "Optimization finished!" << std::endl;

        exit ( EXIT_SUCCESS );
    }
    catch ( ROADEF11::DoNotContinue& e )
    {
        exit ( EXIT_SUCCESS );
    }
    catch( ROADEF11::ParseException& e )
    {
        std::cerr << "Error while parsing: " << e.what() << std::endl;
    }
    catch ( ROADEF11::InvalidSolution& e )
    {
        std::cerr << "The produced solution is not valid" << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
    
    exit ( EXIT_FAILURE );
}
