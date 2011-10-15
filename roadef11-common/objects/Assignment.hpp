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
#ifndef __roadef11_ASSIGNMENT_HPP
#define __roadef11_ASSIGNMENT_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11
#include "roadef11-common/service/ServiceExceptions.hpp"
#include "roadef11-common/Types.hpp"
#include "roadef11-common/objects/Assignment.hpp"
#include "roadef11-common/objects/Parameters.hpp"
#include "roadef11-common/util/Util.hpp"
#include "roadef11-common/Types.hpp"
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
///////////////////////////////////////////////////////////////////////////
// STD
#include <stdexcept>
#include <list>
#include <set>
#include <fstream>
///////////////////////////////////////////////////////////////////////////
namespace ROADEF11
{
    class Parameters;

    /**
     * Process to machine assignments. All operations are constant time
     * except for the parsing method since it pre-computes some values
     * (processes per machine, per service,...).
     *
     * @author daniperez
     * @todo   Use other values for the size of the array or even
     *         other containers.
     */
    class Assignment
    {   
        public:
            
            /**
             * Constructor. Given a file name and parameters,
             * creates an assignment and fill its with the
             * content of the file.
             * 
             * @param fileName File containing the assignments ( an
             *        array of integers representing machines. The position
             *        of the integer is the corresponding process ).
             * @param parameters Parameters the assignment are based up on.
             * @return boost::shared_ptr<Assignment>
             * @throw IOException if the input files cannot be read.
             * @throw ParseException if the input files have wrong format.
             * @todo This should go into a factory.
             */ 
            Assignment ( const char* fileName, const Parameters& parameters )
            throw ( ROADEF11::IOException, ROADEF11::ParseException )
                : _parameters ( parameters )
            {
                throwing_assert ( fileName != NULL );
                
                std::ifstream file ( fileName, std::ios::in );
                
                if ( file.fail() )
                {
                    throw ROADEF11::IOException
                    (
                        (
                            std::string ( "'" ) + fileName +
                            std::string ( "' cannot be open" )
                        ).c_str()
                    );
                }

                ushort machine = 0;
                ushort process = 0;
                
                while ( file >> machine )
                {
                    _processToMachine[process] = machine ;
                    _machineToProcess[machine].push_back(process);
                    _serviceToProcess[_parameters.processes.getService(process)].push_back(process);
                    ++process;
                    
                    if ( file.fail() )
                    {
                        throw ROADEF11::IOException
                        (
                            (std::string(fileName) +
                                std::string (" cannot be read")).c_str()
                        );
                    }
                }

                file.clear();
                file.close();
                
                if ( file.fail() )
                {
                    throw ROADEF11::IOException
                    (
                        (std::string(fileName) +
                            std::string (" cannot be closed")).c_str()
                    );
                }
                
                postProcess();
            }

        public:
            
            /**
             * @name Query methods. 
             */
            ///@{                
             /**
             * Returns the machine assigned to the given process.
             * O(1).
             * 
             * @param processId Process' id.
             * @return Machine assigned to the process.
             */ 
            ushort
            getMachine ( ushort processId ) const
            {
                throwing_assert ( processId < _parameters.processes.size() ) ;
                
                return _processToMachine [ processId ] ;
            }

            /**
             * Returns the processes corresponding to the given machine.
             * O(1).
             * 
             * @param machineId Machine's id.
             * @return list of process ids of the given machine.
             */
            const std::list<ushort>&
            getProcessesPerMachine ( ushort machineId ) const
            {
                throwing_assert ( machineId < _parameters.machines.size() ) ;

                return _machineToProcess [ machineId ];
            }

            /**
             * Returns the machine utilization for a given resource.
             * The utilization was precomputed in the constructor
             * and updated in the methods changing the loaded.
             * O(1).
             *
             * @param machine Machine's id.
             * @param resource Resource id.
             * @return Load of the machine.
             */
            const uint
            getUtilization ( ushort machine, ushort resource ) const
            {
                throwing_assert ( machine < _parameters.machines.size() ) ;
                throwing_assert ( resource < _parameters.resources.size() ) ;

                return _machineLoad [ machine ] [ resource ] ;
            }

            /**
             * Returns the processes corresponding to the given service.
             * O(1).
             * 
             * @param serviceId Service's id.
             * @return list of process ids of the given service.
             */
            const std::list<ushort>&
            getProcessesPerService ( ushort serviceId ) const
            {
                throwing_assert ( serviceId < _parameters.services.size() ) ;

                return _serviceToProcess [ serviceId ];
            }
             
            /**
             * Returns the number of different locations corresponding to
             * the given service. O(1).
             * 
             * @param serviceId Service's id.
             * @return number of locations of the given service.
             */
            const ushort
            getNumLocationsPerService ( ushort serviceId ) const
            {
                throwing_assert ( serviceId < _parameters.services.size() ) ;

                return _serviceToNumLocations [ serviceId ] ;
            }
            ///@}

        public:

            /**
             * @name Methods changing the assignment.
             */
            ///@{
            /**
             * Moves a process from the original machine to the
             * new given machine.
             *
             * @param process Process to migrate.
             * @param newMachine Machine to move the process to.
             */
            void move ( uint process, uint newMachine )
            {
            }
            ///@}

        public:
            
            /**
             * @name I/O methods.
             */
            ///@{
            /**
             * Produces a human-readable representation of an assignment.
             * 
             * @return std::string.
             */
            std::string
            toString () const
            {
                std::string output;
                
                for ( ushort i = 0 ; i < _parameters.processes.size() ; ++i )
                {
                    output += "M(" ;
                    output += boost::lexical_cast<std::string> ( i ) ;
                    output += ") = " ;
                    output += boost::lexical_cast<std::string>
                              (
                                getMachine ( i )
                              ) ;
                    output += "\n" ;
                }
    
                return output;
            }

            /**
             * Writes the assignment to a file.                             
             * 
             * @param fileName File to write to.
             * @throw IOException if the input files cannot be read.
             */ 
            void
            write ( const char* fileName ) const throw ( ROADEF11::IOException)
            {
                std::ofstream output ( fileName, std::ios::out );

                for ( uint i = 0; i < _parameters.processes.size() ; ++i )
                {
                    if ( i > 0 )
                    {
                        output << " ";
                    }

                    output << _processToMachine [ i ];
                }
            }
            ///@}
 
        protected:
                    
            /**
             * Processes a just-parsed assignment. We pre-calculate
             * some data used frequently. O(num_services*num_procs).
             */
            void
            postProcess ()
            {
                for ( ushort serviceId = 0;
                      serviceId < _parameters.services.size();
                      ++serviceId )
                {
                    const std::list<ushort>& procs
                        = getProcessesPerService ( serviceId );
                    
                    std::set<ushort> locations;    
                        
                    BOOST_FOREACH ( ushort proc, procs )
                    {
                        ushort machineId
                            = getMachine ( proc );
                        
                        updateAssignment
                            ( serviceId, proc, machineId, locations );
                    }
                }
            }

            /**
             * Updates the structures depending on an assignment such
             * as utilization and locations per service.
             *
             * @param service Service.
             * @param process Process.
             * @param machine Machine.
             * @param locations Set of locations already processed. Pass
             *        an empty one if none was processes yet.
             */
            void
            updateAssignment
            (
                ushort service,
                ushort process,
                ushort machine,
                std::set<ushort>& locations
            )
            {
                updateServiceLocations ( service, process, machine, locations );
                updateMachineLoad ( process, machine );
            }

            /**
             * Updates the locations per service.
             *
             * @param service Service.
             * @param process Process.
             * @param machine Machine.
             * @param locations Set of locations already processed. Pass
             *        an empty one if none was processes yet.
             */
            void
            updateServiceLocations
            (
                ushort service,
                ushort process,
                ushort machine,
                std::set<ushort>& locations
            )
            {
                throwing_assert ( service < _parameters.services.size() ) ;
                throwing_assert ( process < _parameters.processes.size() ) ;
                throwing_assert ( machine < _parameters.machines.size() ) ;

                int location
                    = _parameters.machines.getLocation ( machine );

                std::set<ushort>::iterator it;

                if ( (it = locations.find ( location )) == locations.end() )
                {
                    locations.insert ( it, location );
                                                
                    _serviceToNumLocations [ service ] += locations.size();
                }
            }

            /**
             * Updates the machine's load with the resource consumption of the
             * given process.
             *
             * @param process Process.
             * @param machine Machine.
             */
            void
            updateMachineLoad ( ushort process, ushort machine )
            {
                throwing_assert ( process < _parameters.processes.size() ) ;
                throwing_assert ( machine < _parameters.machines.size() ) ;

                for ( ushort
                      resource = 0;
                      resource < _parameters.resources.size();
                      ++resource )
                {
                    // TODO: this shouldn't go into objects. 
                    _machineLoad[machine][resource] +=
                        _parameters.processes.getRequirement ( process, resource );
                }
            }

        private:
            
            const Parameters&  _parameters;
            /** */
            ushort             _processToMachine[MAX_NUM_PROCESSES];
            /** */
            std::list<ushort>  _machineToProcess[MAX_NUM_MACHINES];
            /** */
            uint               _machineLoad[MAX_NUM_MACHINES][MAX_NUM_RESOURCES];
            /** */
            std::list<ushort>  _serviceToProcess[MAX_NUM_SERVICES];
            /** */
            ushort             _serviceToNumLocations[MAX_NUM_SERVICES];
    };
};

#endif
