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
#ifndef __roadef12_MACHINES_HPP
#define __roadef12_MACHINES_HPP
///////////////////////////////////////////////////////////////////////////
// roadef12
#include "roadef12-common/objects/model/Resources.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
#include <vector>
#include <stdexcept>
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/lexical_cast.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF12COMMON
{
    /**
     * Machine-related procedures.
     *
     * @author daniperez
     */
    class Machines
    {
        public:
            
            /**
             * Constructor.
             */
            Machines
              ( const Resources& resources, const std::vector<int>& values )
                : _resources ( resources ), _values (values)
            {
            }
            
            /**
             * Number of machines.
             * 
             * @return number of machines.
             */
            uint
            size () const
            {
                return _values [ startIndex() ];
            }

        public:
            
            /**
             * @name Methods related to a single machine
             */
            ///@{
            /**
             * Machine's neighborhood.
             * 
             * @param machineId Machine's id.
             * @return Neighborhood's id.
             */ 
            int
            getNeighborhood ( uint machineId ) const
            {
                assert ( exists ( machineId ) );

                return _values [ getNeighborhoodIndex ( machineId ) ]; 
            }
            
            /**
             * Machine's location.
             * 
             * @param machineId Machine's id.
             * @return Location's id.
             */ 
            int
            getLocation ( uint machineId ) const
            {
                assert ( exists ( machineId ) );

                return _values [ getNeighborhoodIndex ( machineId ) + 1 ]; 
            }
            
            /**
             * @name Methods related to a single machine, per resource
             */
            ///@{                
            /**
             * Returns the machine's capacity of a given resource.
             * 
             * @param machineId Machine's id.
             * @param resourceId Resource's id.
             * @return Capacity.
             */ 
            int
            getCapacity ( uint machineId, uint resourceId ) const
            {
                assert ( exists ( machineId ) );

                assert ( _resources.exists ( resourceId ) );
                
                return _values [ getCapacitiesIndex ( machineId ) + resourceId ]; 
            }
            
            /**
             * Returns the machine's safety capacity of a given resource.
             * 
             * @param machineId Machine's id.
             * @param resourceId Resource's id.
             * @return Safety capacity.
             */ 
            int
            getSafetyCapacity ( uint machineId, uint resourceId ) const
            {
                assert ( exists ( machineId ) );

                assert ( _resources.exists ( resourceId ) );
                
                return _values [ getCapacitiesIndex ( machineId ) +
                                 _resources.size () +
                                 resourceId ]; 
            }
            
            /**
             * Returns the cost of moving machineId to moveToMachineId.
             * 
             * @param machineId Machine's id.
             * @param moveToMachineId Destination machine's id.
             * @return Moving cost.
             */ 
            int
            getMovingCost ( uint machineId, uint moveToMachineId ) const
            {
                assert ( exists ( machineId ) );
                assert ( exists ( moveToMachineId ) );
                
                return _values [ getCapacitiesIndex ( machineId ) +
                                 _resources.size () +
                                 _resources.size() +
                                 moveToMachineId ]; 
            }
            ///@}
            
            /**
             * Returns a human-readable representation of a machine.
             * 
             * @param machineId Machine's id.
             * @return Machine's representation for debugging purposes.
             */
            std::string
            toString ( uint machineId ) const
            {
                assert ( exists ( machineId ) );
                
                std::string output;
                                
                output += "    #" ;
                output += boost::lexical_cast<std::string> ( machineId ) ;
                output += ": n";
                output += boost::lexical_cast<std::string>
                              ( getNeighborhood ( machineId ) ) ;
                output += ", l";
                output += boost::lexical_cast<std::string>
                              ( getLocation ( machineId ) ) ;
                output += "\n";
                output += "        capacities      :  " ;
                for ( uint r = 0 ; r < _resources.size(); ++r )
                {
                    output += "r" ;
                    output += boost::lexical_cast<std::string> ( r );
                    output += ":" ;
                    output += boost::lexical_cast<std::string>
                              ( getCapacity ( machineId, r ) ) ;
                    output += "   ";
                }
                output += "\n";
                output += "        saf. capacities :  " ;
                for ( uint r = 0 ; r < _resources.size(); ++r )
                {
                    output += "r" ;
                    output += boost::lexical_cast<std::string> ( r );
                    output += ":";
                    output += boost::lexical_cast<std::string>
                              ( getSafetyCapacity ( machineId, r ) ) ;
                    output += "   ";
                }
                output += "\n";
                output += "        mov. cost       :  " ;
                for ( uint m = 0 ; m < size(); ++m )
                {
                    output += "m" ;
                    output += boost::lexical_cast<std::string> ( m );
                    output += ":";
                    output += boost::lexical_cast<std::string>
                              ( getMovingCost ( machineId, m ) ) ;
                    output += "   ";
                }
                output += "\n";
                
                return output ;                
            }
            
            /**
             * Says if the given machine exists or not.
             * 
             * @param machineId Machine's id.
             * @return True if the machine exists, false otherwise.
             */ 
            bool
            exists ( uint machineId ) const
            {
                if ( machineId >= size() )
                {
                    std::cerr
                        << std::string ( "Machine with id=" )
                        << boost::lexical_cast<std::string>( machineId )
                        << std::string ( " doesn't exist. " )
                        << std::string ( "Valid id's go from 0 to " )
                        << boost::lexical_cast<std::string> ( size() - 1 );
                        
                   return false;
                }
                else
                {
                   return true; 
                }                
            }
            ///@}
            
        public:
            
            /**
             * @name Machine's index management
             */
            ///@{ 
            /**
             * According to ROADEF's format, where
             * this item's definition start.
             * 
             * @return Index of the start of definition.
             */ 
            uint
            startIndex () const
            {
                return 1 +  _resources.size()*_resources.getDefinitionSize();
            }
                          
            /**
             * Returns the fixed size of the machine's definition.
             * 
             * @return The number of integers it takes to define a machine.
             */
            uint
            getDefinitionSize () const
            {
                return 1 + 1 + _resources.size() + _resources.size() +
                      size();
            }
             
            /**
             * Returns the location of the neighborhood in this._values.
             * 
             * @param machineId Machine's id.
             * @return Index of its the neightborhood.
             */ 
            uint
            getNeighborhoodIndex ( uint machineId ) const
            {
                // We jump size
                uint idx = startIndex() + 1;
                
                // We jump the previous machines
                idx += machineId*getDefinitionSize();
                
                return idx;
            }
            
            /**
             * Returns the location of the machine's capacities in this._values.
             * 
             * @param machineId Machine's id.
             * @return Index of its the neightborhood.
             */
            uint
            getCapacitiesIndex ( uint machineId ) const
            {
                return getNeighborhoodIndex ( machineId ) + 1 + 1 ;
            }
            ///@}
            
        private:
            
            const Resources&        _resources;
            const std::vector<int>& _values;
    };
};

#endif
