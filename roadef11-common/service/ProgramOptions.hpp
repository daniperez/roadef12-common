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
#ifndef __roadef11_PROGRAM_OPTIONS_HPP
#define __roadef11_PROGRAM_OPTIONS_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11 Service
#include <roadef11-common/service/Service.hpp>
///////////////////////////////////////////////////////////////////////////
// STD
#include <iostream>
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/program_options.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF11
{   
    /**
     * Methods to parse command-line options.
     *
     * @author daniperez 
     */
    class ProgramOptions
    {
        public:
            
        /**
         * Parses the given command-line with the recognized options (see
         * ROADEF11::ProgramOptions::createCommandLineOptions).
         * 
         * @param argc Number of arguments in argv.
         * @param argv Command-line switches including program name.
         * @return ROADEF11::ServiceInput.
         */    
        static ROADEF11::ServiceInput
        parseCommandLine ( int argc, char** argv )
          throw
          (
              ROADEF11::InvalidParametersException,
              ROADEF11::IOException,
              ROADEF11::DoNotContinue
          )
        {   
            ROADEF11::ServiceInput input;
            
            
            boost::program_options::options_description options
                = createCommandLineOptions ( input );
            
            // This should be achieved by means of boost::program_options
            checkForSpecialSwitches( argc, argv, options );
            
            boost::program_options::variables_map vm;

            try
            {
                boost::program_options::store
                (
                    boost::program_options::command_line_parser( argc, argv )
                        .options ( options ).run (),
                    vm
                );
            }
            catch( boost::program_options::unknown_option& e )
            {
                throw ROADEF11::InvalidParametersException
                (
                    e.what()
                );
            }

            boost::program_options::notify ( vm );
            
            return processParsedOptions( vm, options, argc, argv, input );
        }
        
        protected:

        /**
         * Processes the parsed options (vm): find invalid combinations and
         * the like.
         * 
         * @param vm Parsed options.
         * @param options Options description.
         * @param argc Number of arguments in argv.
         * @param argv Command-line switches including program name.
         * @param input ROADEF11::ServiceInput output argument.
         */     
        static ROADEF11::ServiceInput&
        processParsedOptions
        (
            const boost::program_options::variables_map& vm,
            const boost::program_options::options_description& options,
            const int argc,
            char** argv,
            ROADEF11::ServiceInput& input
        )
          throw
          (
              ROADEF11::InvalidParametersException,
              ROADEF11::IOException,
              ROADEF11::DoNotContinue
          )
        {
            if ( vm.count("help") )
            {
                std::cout << argv[0] << " [options] <executable> " << std::endl;
                std::cout << options << std::endl;
                throw ROADEF11::DoNotContinue();
            }
            else if ( vm.count("name") )
            {
                std::cout << ROADEF11::Service::team() << std::endl;
                throw ROADEF11::DoNotContinue();
            }
            else if ( vm.count("verbose") )
            {
                std::cout << ROADEF11::Service::team() << std::endl;
                throw ROADEF11::DoNotContinue();
            }
            else
            {
                if ( vm.count("parameters") == 0 ||
                    vm.count("input-solution") == 0 ||
                    vm.count("output") == 0 ||
                    vm.count("time") == 0 || // 'time' and 'seed' have their
                    vm.count("seed") == 0 )  // own defaults, this is not
                {                             // really needed.
                    throw ROADEF11::InvalidParametersException
                            ( "-p, -i and -o parameters are mandatory" );
                }
            }

            return input;
        }

        /**
         * Checks for special switches such as empty list or -name.
         * 
         * @param argc Number of arguments in argv.
         * @param argv Command-line switches including program name.
         * @param options Accepted options.
         * @throw ROADEF11::DoNotContinue if the application can finish
         *                                after calling this method.
         */
        static
        void checkForSpecialSwitches
        (
            int argc,
            char** argv,
            const boost::program_options::options_description& options
        )
          throw ( ROADEF11::DoNotContinue )
        {
            if ( argc == 1 )
            {
                std::cout << argv[0] << " [options] <executable> " << std::endl;
                std::cout << options << std::endl;
                throw ROADEF11::DoNotContinue();
            }
            else
            {
                for ( int i = 1; i < argc; ++i )
                {
                    if ( std::string ( argv[i] ) == "-name" )
                    {
                        std::cout << ROADEF11::Service::team() << std::endl;
                        
                        throw ROADEF11::DoNotContinue();
                    }
                }
            }
        }
        
        /**
         * Creates boost's command-line options.
         * 
         * @param input Service's input to fill in.
         * @throw ROADEF11::InvalidParametersException if a non-recognized
         *        parameter is passed.
         */
        static boost::program_options::options_description
        createCommandLineOptions ( ROADEF11::ServiceInput& input )
          throw ( ROADEF11::InvalidParametersException )
        {
            boost::program_options::options_description
                genericOptionsDesc ( "Generic options" );
            genericOptionsDesc.add_options()
            ("help,h", "produces this help message")
            ("verbose,v", "produce debug output");

            boost::program_options::options_description
                roadefOptionsDesc ( "Roadef's mandatory options" );
            roadefOptionsDesc.add_options ()
            (
                "name,n",
                "prints the team's name. -name also accepted."
            )
            (
                "time,t",
                boost::program_options::value<unsigned int>( &input.secondsTimeLimit )->default_value(5*60),
                "seconds to run the program"
            )
            (
                "parameters,p",
                boost::program_options::value<std::string>( &input.parameters ),
                "input file"
            )
            (
                "input-solution,i",
                boost::program_options::value<std::string>( &input.referenceSolution ),
                "input solution file"
            )
            (
                "output,o",
                boost::program_options::value<std::string>( &input.solution ),
                "output file to write the solution to"
            )
            (
                "seed,s",
                boost::program_options::value<unsigned int>( &input.seed )->default_value(0),
                "seed for randomized algorithms"
            );
            
            boost::program_options::options_description
                extraOptionsDesc ( "Optimization options" );
            extraOptionsDesc.add_options()
                ( "null,u",
                  boost::program_options::value<bool>( &input.nullNull )
                    ->zero_tokens()->default_value(false),
                  "No optimization, no output is produced"
                )
                ( "null-copy,c",
                  boost::program_options::value<bool>( &input.nullCopy )
                    ->zero_tokens()->default_value(false),
                  "No optimization, copies input"
                );
            
            // Wrap-up
            boost::program_options::options_description allOptionsDesc;
            allOptionsDesc.add ( genericOptionsDesc )
                        .add ( roadefOptionsDesc )
                        .add ( extraOptionsDesc );
                        
            return allOptionsDesc;
        }
    };
};

#endif
