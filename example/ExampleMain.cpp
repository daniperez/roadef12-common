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
// roadef12 
#include "roadef12-common/service/Service.hpp"
#include "roadef12-common/service/ProgramOptions.hpp"
#include "roadef12-common/service/ServiceExceptions.hpp"
#include "roadef12-common/util/Util.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
#include <iostream>
///////////////////////////////////////////////////////////////////////////

class ExampleService : public ROADEF12COMMON::Service
{
    public:

        ExampleService ( const ROADEF12COMMON::ServiceInput& input )
            throw ( ROADEF12COMMON::IOException, ROADEF12COMMON::ParseException )
            : ROADEF12COMMON::Service ( input )
        {
        }

        virtual void optimize ()
          throw ( ROADEF12COMMON::IOException, ROADEF12COMMON::ParseException )
        {
            std::cout << "processes=" << params.processes.size()
                      << " machines=" << params.machines.size()
                      << " resources=" << params.resources.size()
                      << std::endl;

            if ( options.nullCopy )
            {
                firstAssignment.write ( options.solution.c_str() );
            }
            else if ( options.nullNull )
            {
                // Do not perform any optimization, no output is produced.
            }
            else
            {
                std::cout << "Your code goes here" << std::endl;

                // Don't forget to write a solution
                firstAssignment.write ( options.solution.c_str() );
            }
        }
};

int main ( int argc, char **argv )
{
    try
    {
        ROADEF12COMMON::ServiceInput input
            = ROADEF12COMMON::ProgramOptions::parseCommandLine ( argc, argv );
            
        ExampleService service ( input );

        std::cout << "Optimization started..." << std::endl;

        service.optimize();
        
        std::cout << "Optimization finished!" << std::endl;

        exit ( EXIT_SUCCESS );
    }
    catch ( ROADEF12COMMON::DoNotContinue& e )
    {
        exit ( EXIT_SUCCESS );
    }
    catch( ROADEF12COMMON::ParseException& e )
    {
        std::cerr << "Error while parsing: " << e.what() << std::endl;
    }
    catch ( ROADEF12COMMON::InvalidSolution& e )
    {
        std::cerr << "The produced solution is not valid" << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
    
    exit ( EXIT_FAILURE );
}
