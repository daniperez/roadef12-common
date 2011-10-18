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
#ifndef __roadef11_SERVICE_HPP
#define __roadef11_SERVICE_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11 material 
#ifdef USE_CHECKER 
#include "roadef11-material/solution_checker/solution_checker.h"
#endif
///////////////////////////////////////////////////////////////////////////
// roadef11 Service
#include "roadef11-common/service/ServiceExceptions.hpp"
#include "roadef11-common/objects/Parameters.hpp"
#include "roadef11-common/objects/Assignment.hpp"
#include "roadef11-common/commands/FileParser.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
#include <vector>
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/shared_ptr.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF11
{
    /**
     * Service's input.
     * 
     * @author daniperez
     * @todo Remove roadef11/bom/Assignment.hpp dependency in Service.cpp
     * @todo Model.hpp is BOM, only pointers to it should be used.
     */
    struct ServiceInput
    {
        /**
         * Max number of seconds to run (-t switch)
         */ 
        unsigned int secondsTimeLimit;
        
        /**
         * File containing the parameters (-p switch)
         */
        std::string parameters;
        
        /**
         * File containing the reference solution (-i switch)
         */ 
        std::string referenceSolution;
        
        /**
         * File to write the solution to (-o switch)
         */
        std::string solution;
        
        /**
         * Seed (-s switch)
         */ 
        unsigned int seed;
        
        /**
         * No optimization will be performed but input solution
         * will be copied to output (-c switch).
         */
        bool nullCopy;
        
        /**
         * No optimization will be performed, no output will be
         * produced (-u switch).
         */
        bool nullNull;
    };    
    
    /**
     * Service (aka 'API').
     * 
     * @author daniperez 
     */
    class Service
    {
        public:
            
            /**
             * Constructor.
             * 
             * @param input Arguments of the optimization.
             * @throw IOException if the input files cannot be read.
             * @throw ParseException if the input files have wrong format.
             */
            Service ( const ROADEF11::ServiceInput& input )
              throw ( ROADEF11::IOException, ROADEF11::ParseException )
                : options  ( parseInput ( input ) ),
                  params   ( _parametersArray ),
                  firstAssignment
                  (
                      input.referenceSolution.c_str(),
                      params
                  )
            {
            }
            
            /**
             * Returns the team's name.
             *
             * @return Team's name.
             */    
            static const char* team ()
            {
                return "a-team";
            }
    
            /**
             * API entry point. Optimizes according to the given
             * arguments.
             *
             * @throws IOException if an I/O error occurred while
             *                     reading an instance or producing
             *                     the output.
             * @throws ParseException if the instance contain syntactic
             *                        errors.
             */
            virtual void optimize () throw ( IOException, ParseException )
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
                    firstAssignment.write ( options.solution.c_str() );
                }
            }

            /**
             * Uses the checker, as provided by ROADEF, to check the
             * solution. Returns the objective function value. This
             * is a convenient method to check the solution programmatically,
             * but the checker can also be used (see the main CMakeLists.txt).
             *
             * <b>IMPORTANT:</b> Only compiled if USE_CHECKER symbol
             * is defined for library delivery reasons.
             *
             * @return Objective function value.
             * @throw InvalidSolution If the solution is not valid.
             * @throw IOException if the input files cannot be read.
             * @throw ParseException if the input files have wrong format.
             */ 
            long check () throw ( ROADEF11::InvalidSolution,
                                  ROADEF11::IOException,
                                  ROADEF11::ParseException )
            {
                #ifdef USE_CHECKER                
                using roadef_challenge::DataParser;
                using roadef_challenge::SolutionChecker;

                vector<int> model;
                vector<int> initial_assignments;
                vector<int> new_assignments;

                FileParser::parseVector ( options.parameters.c_str(), model );
                FileParser::parseVector ( options.referenceSolution.c_str(),
                                          initial_assignments );
                FileParser::parseVector ( options.solution.c_str(), new_assignments );

                DataParser data(model,
                                initial_assignments,
                                new_assignments);

                SolutionChecker solution_checker(data.machines(),
                                                data.services(),
                                                data.processes(),
                                                data.balance_costs(),
                                                data.process_move_cost_weight(),
                                                data.service_move_cost_weight(),
                                                data.machine_move_cost_weight(),
                                                data.initial_assignments(),
                                                data.new_assignments());

                if (solution_checker.Check())
                {
                    return solution_checker.GetObjectiveCost();
                }
                else
                {
                    throw ROADEF11::InvalidSolution();
                }
                #else
                return 0;
                #endif
            }
            
        public:    
                 
            /**
             * Returns a string representing the parsed input.
             * 
             * @return std::string.
             */
            std::string toString ()
            {
                std::stringstream output;
                
                output << "Instance: " << std::endl;
                
                output << "  Resources (" << params.resources.size() << ")" << std::endl;
                for ( uint i = 0 ; i < params.resources.size() ; ++i )
                {
                    output << params.resources.toString ( i );
                }
                output << "  Machines (" << params.machines.size() << ")" << std::endl;
                for ( uint i = 0 ; i < params.machines.size() ; ++i )
                {
                    output << params.machines.toString ( i );
                }
                output << "  Services (" << params.services.size() << ")" << std::endl;
                for ( uint i = 0 ; i < params.services.size() ; ++i )
                {
                    output << params.services.toString ( i );
                }
                output << "  Processes (" << params.processes.size() << ")" << std::endl;
                for ( uint i = 0 ; i < params.processes.size() ; ++i )
                {
                    output << params.processes.toString ( i );
                }
                output << "  Costs" << std::endl;
                output << params.costs.toString ();
                
                return output.str();
            }

        protected:    
                     
            /**
             * Parses the model parameters.
             * Returns input parameter for convenience.
             * 
             * @param input Input to be parsed.
             * @throw IOException if the input files cannot be read.
             * @throw ParseException if the input files have wrong format.
             */
            const ServiceInput&
            parseInput ( const ServiceInput& input )
            throw ( ROADEF11::IOException, ROADEF11::ParseException )
            {
                FileParser::parseVector ( input.parameters.c_str(), _parametersArray );
                
                return input;
            }
            
        private:
             
             // WARNING: this must be declared before options
             std::vector<int>                    _parametersArray;
             
        public:

            /**
             * The parsed command line options.
             */           
            const ROADEF11::ServiceInput&       options;
            
            /**
             * Problem parameters.
             */ 
            const Parameters                    params;
             
            // WARNING: this must be declared after params.
            //          If a shared_ptr were passed to assignment,
            //          we could use any order.
            /**
             * First process to machine  assignment.
             */
            const Assignment                    firstAssignment;
    };

};

#endif
