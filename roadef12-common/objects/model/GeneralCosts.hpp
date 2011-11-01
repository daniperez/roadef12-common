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
#ifndef __roadef12_GENERAL_COSTS_HPP
#define __roadef12_GENERAL_COSTS_HPP
///////////////////////////////////////////////////////////////////////////
// roadef12
#include "roadef12-common/objects/model/Processes.hpp"
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
     * General costs.
     *
     * @author daniperez
     */
    class GeneralCosts
    {
        public:
            
            /**
             * Constructor.
             */
            GeneralCosts (
                        const Processes& processes,
                        const Resources& resources,
                        const std::vector<int>& values )
                : _processes ( processes ),
                  _resources ( resources ),
                  _values (values)
            {
            }

            /**
             * Returns a human-readable representation of the costs.
             * 
             * @return Cost's representation for debugging purposes.
             */
            std::string
            toString () const
            {
                std::string output;
                                
                output += "    Balance costs (" ;
                output += boost::lexical_cast<std::string>
                            ( getNumObjectiveBalance() ) ;
                output += ")" ;
                output += "\n";
                
                for ( uint c = 0 ; c < getNumObjectiveBalance(); ++c )
                {
                    output += "        #";
                    output += boost::lexical_cast<std::string> ( c ) ;
                    output += "              : " ;
                    output += boost::lexical_cast<std::string> (
                        getBalanceCostWeight ( c ) ) ;
                    output += "%, <r" ;
                    output += boost::lexical_cast<std::string> (
                        getObjectiveBalanceR1 ( c ) ) ;
                    output += ",r" ;
                    output += boost::lexical_cast<std::string> (
                        getObjectiveBalanceR2 ( c ) ) ;
                    output += "," ;
                    output += boost::lexical_cast<std::string> (
                        getObjectiveBalanceTarget ( c ) ) ;
                    output += ">" ;
                }

                output += "\n";
                
                output += "    Process mov. weight : " ;
                output +=  boost::lexical_cast<std::string> (
                    getProcessMoveCostWeight() ) ;
 
                output += "\n";
                
                output += "    Service mov. weight : " ;
                output +=  boost::lexical_cast<std::string> ( 
                    getServiceMoveCostWeight() ) ;
                
                output += "\n";
                
                output += "    Machine mov. weight : " ;
                output +=  boost::lexical_cast<std::string> (
                    getMachineMoveCostWeight() ) ;
                
                output += "\n";
                 
                return output ;
                
                return "" ;
            }
            
            /**
             * @name Objective balance
             */
            ///@{
            /**
             * Returns the number of objective balance constraints.
             *
             * @return Number of objective balance constraints.
             */    
            uint
            getNumObjectiveBalance() const
            {
                return _values [ startIndex() ];
            }
            
            /**
             * Balance's first resource.
             * 
             * @param balanceId Balance's id.
             * @return Id of first resource of balance.
             */ 
            int
            getObjectiveBalanceR1 ( uint balanceId ) const
            {
                assert ( objectiveBalanceExists ( balanceId ) );

                return _values [ startIndex() + 1 +
                                 balanceId*getBalanceDefinitionSize() ];
            }
            
            /**
             * Balance's second resource.
             * 
             * @param balanceId Balance's id.
             * @return Id of second resource of balance.
             */ 
            int
            getObjectiveBalanceR2 ( uint balanceId ) const
            {
                assert ( objectiveBalanceExists ( balanceId ) );

                return _values [ startIndex() + 1 +
                                 balanceId*getBalanceDefinitionSize() + 1 ];
            }
            
            /**
             * Balance's target.
             * 
             * @param balanceId Balance's id.
             * @return Target of the balance.
             */ 
            int
            getObjectiveBalanceTarget ( uint balanceId ) const
            {
                assert ( objectiveBalanceExists ( balanceId ) );

                return _values [ startIndex() + 1 +
                                 balanceId*getBalanceDefinitionSize() +
                                 1 + 1 ];
            }
            
            /**
             * Returns the weight of the given balance cost.
             * 
             * @param balanceId Balance's id.
             * @return weight from 0 to 100 of the balance cost.
             */ 
            uint
            getBalanceCostWeight( uint balanceId ) const
            {
                return _values [ startIndex() + 1 +
                                 balanceId*getBalanceDefinitionSize() +
                                 1 + 1 + 1];
            }

            /**
             * Says if the given objective balance exists.
             * 
             * @param balanceId Objective balance's id.
             * @return True if the obj. balance exists, false otherwise.
             */ 
            bool
            objectiveBalanceExists ( uint balanceId ) const
            {
                if ( balanceId >= getNumObjectiveBalance() )
                {
                    std::cerr
                        << std::string ( "Objective balance with id=" )
                        << boost::lexical_cast<std::string>( balanceId )
                        << std::string ( " doesn't exist. " )
                        << std::string ( "Valid id's go from 0 to " )
                        << boost::lexical_cast<std::string> (
                              getNumObjectiveBalance() - 1 ) ;
                    
                    return false;
                }
                else
                {
                   return true; 
                }                                
            }
            ///@}
            
            /**
             * @name Cost weights
             */
            ///@{
            /**
             * Returns the weight of the process move in the cost
             * function, from 0 to 100.
             * 
             * @return Process move cost's weight.
             */
            uint
            getProcessMoveCostWeight () const
            {
                return _values [
                    startIndex() + 1 +
                    getBalanceDefinitionSize()*getNumObjectiveBalance() ] ;
            }
            
            /**
             * Returns the weight of the service move in the cost
             * function, from 0 to 100.
             * 
             * @return Service move cost's weight.
             */
            uint
            getServiceMoveCostWeight () const
            {
                return _values [
                    startIndex() + 1 +
                    getBalanceDefinitionSize()*getNumObjectiveBalance() + 1 ] ;
            }
                        
            /**
             * Returns the weight of the machine move in the cost
             * function, from 0 to 100.
             * 
             * @return Machine move cost's weight.
             */
            uint
            getMachineMoveCostWeight () const
            {
                return _values [
                    startIndex() + 1 +
                    getBalanceDefinitionSize()*getNumObjectiveBalance() +
                    1 + 1 ] ;
            }
                       
            /**
             * Weight of the resource's load cost.
             * 
             * @param resourceId Resource's id.
             * @return Resource's load cost weight from 0 to 100.
             */ 
            int
            getLoadCostWeight ( uint resourceId ) const
            {
                assert ( _resources.exists ( resourceId ) );

                return
                    _values
                    [
                      startIndex()+
                      1+
                      resourceId*_resources.getDefinitionSize()+
                      1
                    ]; 
            }
            ///@}
            
      public:
            
            /**
             * @name Process' index management
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
                return _processes.startIndex() + 1 +
                       _processes.size()*_processes.getDefinitionSize();
            }
            
            /**
             * Returns the fixed size of an objective balance definition.
             * 
             * @return Fixed size it takes to define an objective balance.
             */
            uint
            getBalanceDefinitionSize () const
            {
                return 4;
            }
            ///@}
            
        private:
            
            const Processes&        _processes;
            const Resources&        _resources;
            const std::vector<int>& _values;
    };
};

#endif
