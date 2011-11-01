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
#ifndef __roadef12_PROCESSES_HPP
#define __roadef12_PROCESSES_HPP
///////////////////////////////////////////////////////////////////////////
// roadef12
#include "roadef12-common/objects/model/Resources.hpp"
#include "roadef12-common/objects/model/Machines.hpp"
#include "roadef12-common/objects/model/Services.hpp"
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
     * Process-related procedures.
     *
     * @author daniperez
     */
    class Processes
    {
        public:
            
            /**
             * Constructor.
             */
            Processes ( const Resources& resources,
                      const Services& services,
                      const std::vector<int>& values )
                : _resources ( resources ),
                  _services ( services ),
                  _values (values)
            {
            }
            
            /**
             * Number of processes.
             * 
             * @return number of processes.
             */
            uint
            size () const
            {
                return _values [ startIndex() ];
            }

        public:
            
            /**
             * @name Methods related to a single process
             */
            ///@{                
            /**
             * Returns the service this process belongs to.
             * 
             * @param processId Process' id.
             * @return Id of the service.
             */ 
            int
            getService ( uint processId ) const
            {                
                return _values [ getServiceStartIndex ( processId ) ]; 
            }
            
            /**
             * Returns the PMC (process move cost) of the given process.
             * 
             * @param processId Process' id.
             * @return PMC.
             */ 
            int
            getPMC ( uint processId ) const
            {
                return _values [ getResourcesStartIndex( processId ) +
                                 _resources.size() ]; 
            }

            /**
             * @name Methods related to a single process, per resource
             */
            ///@{                
            /**
             * Returns the requirements of the given resource for
             * the given process.
             *
             * @param processId Process' id.
             * @param resourceId Resource's id.
             * @return Requirements of the given resource.
             */ 
            int
            getRequirement ( uint processId, uint resourceId ) const
            {
                return _values [ getResourcesStartIndex( processId ) +
                                 resourceId ]; 
            }
            ///@}
            
            /**
             * Returns a human-readable representation of a process.
             * 
             * @param processId Process' id.
             * @return Process' representation for debugging purposes.
             */
            std::string
            toString ( uint processId ) const
            {
                assert ( exists ( processId ) );
                
                std::string output;
                                
                output += "    #" ;
                output += boost::lexical_cast<std::string> ( processId ) ;
                output += ": s";
                output += boost::lexical_cast<std::string>
                              ( getService ( processId ) ) ;
                output += ", pmc=";
                output += boost::lexical_cast<std::string>
                              ( getPMC ( processId ) ) ;
                output += "\n";
                output += "        needs           : " ;
                for ( uint r = 0 ; r < _resources.size(); ++r )
                {
                    output += "r" ;
                    output += boost::lexical_cast<std::string>
                              ( r ) ;
                    output += ":" ;
                    output += boost::lexical_cast<std::string>
                              ( getRequirement ( processId, r ) ) ;
                    output += "   ";
                }
                output += "\n";
                
                return output ;                
            }
            
            /**
             * Says if the given process exists or not.
             * 
             * @param processId Process's id.
             * @return True if the process exists, false otherwise.
             */ 
            bool
            exists ( uint processId ) const
            {
                if ( processId >= size() )
                {
                    std::cerr
                        << std::string ( "Process with id=" )
                        << boost::lexical_cast<std::string>( processId )
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
                return _services.startIndex() +
                       _services.getAllServicesDefinitionSize();
            }
                          
            /**
             * Returns the fixed size of the machine's definition.
             * 
             * @return The number of integers it takes to define a machine.
             */
            uint
            getDefinitionSize () const
            {
                return 1 + _resources.size() + 1;
            }

            /**
             * Returns the index of the service of the given
             * process.
             * 
             * @param processId Process' id.
             * @return Index of process' service.
             */ 
            uint
            getServiceStartIndex ( uint processId ) const
            {
                return startIndex() + 1 + processId*getDefinitionSize();
            }
            
            /**
             * Returns the index of the first resource of the given
             * process.
             * 
             * @param processId Process' id.
             * @return Index of the firt resource used by the process.
             */
            uint
            getResourcesStartIndex ( uint processId )  const
            {
                return getServiceStartIndex ( processId) + 1;    
            }
            ///@}
            
        private:
            
            const Resources&        _resources;
            const Services&         _services;
            const std::vector<int>& _values;
    };
};

#endif
