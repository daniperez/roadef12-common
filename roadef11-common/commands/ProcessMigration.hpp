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
#ifndef __roadef11_PROCESS_MIGRATION_HPP
#define __roadef11_PROCESS_MIGRATION_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11
#include "roadef11/bom/Parameters.hpp"
#include "roadef11/bom/Assignment.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/foreach.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF11
{
    /**
     * Validation and cost methods for migrating processes.
     * 
     * @author daniperez
     */
    class ProcessMigration
    {
        public:
            
            /**
             * Constructor.
             */
            ProcessMigration ( const Parameters& parameters, Assignment& assignment )
                : _parameters ( parameters ), _assignment ( assignment )
            {
                
            }
            
            /**
             * @name Constraints.
             */
            ///@{                

            /**
             * Checks "(1) Capacity constraints". O(num_resources),
             * very fast since we have 20 resources maximum.
             *
             * @param process Process to migrate.
             * @param machineDest Destination machine.
             * @return True if the move is legal, false otherwise.
             */
            bool
            satisfiesCapacityConstraints ( ushort process, ushort machineDest )
            {
                for
                (
                    uint
                    resource = 0;
                    resource < _parameters.resources.size();
                    ++resource
                )
                {
                    if ( ! satisfiesUsageConstraint
                            ( process, machineDest, resource ) )
                    {
                        return false;
                    }
                }

                return true;
            }

            /**
             * Checks "(2) Conflict Constraints": processes of the same
             * service must be run in distinct machines. O(num_processes)
             * but much faster since it depends only on the number of processes
             * of a single service.
             * 
             * @param process Process to move. 
             * @param machineDest Destination machine.
             * @return True if the move is legal, false otherwise.
             */
            bool
            satisfiesConflictConstraints ( ushort process, ushort machineDest )
            {
                auto service = _parameters.processes.getService ( process );
                
                const std::list<ushort>& serviceProcesses
                    = _assignment.getProcessesPerService ( service );

                BOOST_FOREACH ( auto otherProcess, serviceProcesses )
                {
                    auto machine = _assignment.getMachine ( otherProcess );
                    
                    if ( machineDest == machine &&  process != otherProcess )
                    {
                        return false;
                    }
                }
                
                return true;
            }
            
            /**
             * Checks "(3) Spread Constraints": machines used by a service
             * must spread over at least spreadMin locations. O(num_processes)
             * but much faster since it depends only on the number of processes
             * of a single service.
             * 
             * @param process Process to move. 
             * @param machineDest Destination machine.
             * @return True if the move is legal, false otherwise.
             */
            bool
            satisfiesSpreadConstraints ( ushort process, ushort machineDest )
            {
                auto machineDestLocation
                    = _parameters.machines.getLocation ( machineDest );
                    
                auto service = _parameters.processes.getService ( process );
                    
                const std::list<ushort>& serviceProcesses
                    = _assignment.getProcessesPerService ( service );
                
                auto numLocations
                    = _assignment.getNumLocationsPerService ( service );
                
                int minSpread = _parameters.services.get ( service )->minSpread;
                
                if ( numLocations > minSpread )
                {
                    // If the number of locations is bigger than minSpread,
                    // the new machineDest will certainly satisfy the constraint
                    // regardless of its location. 
                    return true;
                }
                else if ( numLocations == minSpread )
                {
                    // We have to check that we didn't loose 1 distinct location.
                    auto machineOrig = _assignment.getMachine ( process );
                    
                    if ( machineDestLocation
                            == _parameters.machines.getLocation ( machineOrig ) )
                    {
                        // Location didn't change.
                        return true;
                    }
                    else
                    {
                        // Location changed: it only needs to be different
                        // than any other else.
                        return isDifferentLocation
                               ( serviceProcesses, machineDestLocation );                 
                    }
                }
                else if ( numLocations == minSpread - 1 )
                {
                    // We have to win 1 distinct location.
                    return isDifferentLocation
                           ( serviceProcesses, machineDestLocation );  
                }
                else
                {
                    return false;
                }
            }
            
            /**
             * Checks "(4) Dependency Constraints".
             * O(num_services*num_processes) but low number of services since
             * we can expect that  a service cannot depend on all the services
             * and low number of processes since we can expect that a service
             * doesn't contain all the processes.
             * 
             * @param process Process to migrate.
             * @param machineDest Destination machine.
             * @return True if the move is legal, false otherwise.
             */
            bool
            satisfiesDependencyConstraints ( ushort process, ushort machineDest )
            {
                auto service_a_id
                    = _parameters.processes.getService ( process );

                auto service_a
                    = _parameters.services.get ( service_a_id );

                BOOST_FOREACH (
                    int  service_b_id,
                    service_a->deps )
                {

                  auto service_b
                      = _parameters.services.get ( service_b_id );

                  if ( ! satisfiesSingleDependencyConstraint
                          ( service_a, service_b, machineDest ) )
                  {
                    return false;
                  }
                }

                return true;
            }
            
            /**
             * Checks "(5) Transient usage constraint". Note
             * than only the usage of the transient resources
             * are considered.
             *
             * @param process Process to migrate.
             * @param machineDest Destination machine.
             * @return True if the move is legal, false otherwise.
             */
            bool
            satisfiesTransientOnlyUsageConstraints
                ( ushort process, ushort machineDest )
            {
                for
                (
                    uint resource = 0;
                    resource < _parameters.resources.size();
                    ++resource
                )
                {
                    if ( _parameters.resources.isTransient ( resource )
                            &&  ! satisfiesUsageConstraint
                                    ( process, machineDest, resource ) )
                    {
                        return false;
                    }
                }

                return true;
            }

            /**
             * Checks if a process doesn't exceeds the capacity
             * of the destination machine for the given resource. 
             *
             * @param process Process to migrate.
             * @param machineDest Destination machine.
             * @param resource Resource.
             * @return True if the move is legal, false otherwise.
             */
            bool satisfiesUsageConstraint
                ( ushort process, ushort machineDest, ushort resource )
            {
                uint limit
                    = _parameters.machines
                                 .getCapacity ( machineDest, resource );

                uint load
                    = _assignment.getUtilization ( machineDest, resource );

                uint processConsumption
                    = _parameters.processes
                        .getRequirement ( process, resource );

                if ( load + processConsumption > limit )
                {
                    return false;
                }
                else
                {
                    return false;
                }
            }

            /**
             * Cost of moving a process to a new machine.
             *
             * @param process Process to move.
             * @return Cost of moving to the new machine.
             */
            int costOfMigration ( ushort process )
            {
              return _parameters.processes.getPMC ( process );
            }

        private:

            /**
             * Checks "(4) Dependency Constraints" for a single dependent service_b
             * of service_a. O(num_processes) but since a service doesn't contain
             * all the processes, expect it to be low.
             *
             * @param service_a Service to check.
             * @param service_b Service service_a depends up on.
             * @param machineDest Destination machine.
             * @return True if both services satisfy the dependency constraint.
             *         False otherwise.
             */
            bool
            satisfiesSingleDependencyConstraint
            (
                boost::shared_ptr<const Services::Service> service_a,
                boost::shared_ptr<const Services::Service> service_b,
                ushort machineDest
            )
            {
                auto neighborhoodDest
                    = _parameters.machines.getNeighborhood ( machineDest );

                BOOST_FOREACH(
                    int b_process,
                    _assignment.getProcessesPerService( service_b->id ) )
                {
                    auto b_process_machine
                        = _assignment.getMachine( b_process );

                    auto neighborhood_b_process
                        = _parameters
                            .machines
                            .getNeighborhood ( b_process_machine );

                    if ( neighborhood_b_process == neighborhoodDest )
                    {
                      return true;
                    }
                }

                return false;
            }

            ///@}
            
        public:

            /**
             * Says if the processes in serviceProcesses have different
             * locations than the new location machineDestLocation.
             * 
             * @param serviceProcesses Processes of a given service.
             * @param machineDestLocation New location to check.
             * @return True if the locations of the processes are
             *         different than the machineDestLocation. False
             *         otherwise.
             */
            bool
            isDifferentLocation ( const std::list<ushort>& serviceProcesses,
                                  ushort machineDestLocation )
            {
                BOOST_FOREACH ( auto otherProcess, serviceProcesses )
                {
                    auto machine = _assignment.getMachine ( otherProcess );
                    
                    if ( machineDestLocation 
                            == _parameters.machines.getLocation ( machine ) )
                    {
                        return false;
                    }
                }
                
                return true;                
            }
            
        private:
            
            const Parameters& _parameters;
            Assignment& _assignment;
    };    
};

#endif
