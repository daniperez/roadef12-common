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
#ifndef __roadef11_SERVICES_HPP
#define __roadef11_SERVICES_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11
#include "roadef11-common/objects/model/Resources.hpp"
///////////////////////////////////////////////////////////////////////////

namespace ROADEF11
{
    /**
     * Service-related procedures.
     *
     * @author daniperez
     */
    class Services
    {
        public:
            
            /**
            * Abstraction of a service. Services needs of a holder class
            * ("Service", note the lack of "s") because its attributes cannot
            * be retrieved in constant time (they have variable size) and thus
            * they must be stored in Service.
            * 
            * @author daniperez
            */
            struct Service
            {       
                /**
                * Constructor.
                */
                Service ( int id,
                        int minSpread,
                        std::vector<int>::const_iterator firstService,
                        std::vector<int>::const_iterator afterLastService
                        ) : id ( id ), minSpread (minSpread),
                            deps ( firstService, afterLastService )
                {
                }
                
                /**
                 * Service's id.
                 */
                const int        id;
                
                /**
                 * Minimum spread of a service.
                 */
                const int        minSpread;
                
                /**
                 * Services this service dependes up on.
                 */
                std::vector<int> deps;
            };
    
        public:
            
            /**
             * Constructor.
             */
            Services ( const Machines& machines,
                       const std::vector<int>& values )
                : _machines ( machines ),
                  _values ( values ),
                  _definitionSize ( 1 )
            {
                // Services are parsed once into individual Service's since they
                // haven't a fixed size and they cannot be read in constant time.
                uint numServices = _values [ startIndex() ];
                uint numDeps = 0;
                uint serviceId = 0;
                    
                for ( std::vector<int>::const_iterator
                        it  = _values.begin() + startIndex() + 1;
                        it != _values.end() && serviceId < numServices;
                        it += 1+1+numDeps
                    )
                {
                    int minSpread = *it;
                    numDeps = *(it+1);
                    
                    boost::shared_ptr<const Service> servicePtr;
             
                    servicePtr.reset
                    (
                      new Service ( serviceId++, minSpread, it+2, it+2+numDeps )
                    );
                    
                    _services.push_back (  servicePtr );
                    
                    _definitionSize += 2+numDeps;
                }
            }
            
            /**
             * Number of services.
             * 
             * @return number of services.
             */
            uint
            size () const
            {
                return _services.size();
            }

        public:
        
            /**
             * @name Methods related to a single service
             */
            ///@{
            /**
             * Returns a human-readable representation of a service.
             * 
             * @param serviceId service's id.
             * @return Service's representation for debugging purposes.
             */
            std::string
            toString ( uint serviceId ) const
            {
                assert ( exists ( serviceId ) );
                
                const Service& service = *_services[serviceId];
                
                std::string output;
                                
                output += "    #" ;
                output += boost::lexical_cast<std::string> ( serviceId ) ;
                output += ", minSpread=" ;
                output += boost::lexical_cast<std::string> ( service.minSpread ) ;
                output += "\n";
                output += "        depends on      : " ;
                for ( uint s = 0 ; s < service.deps.size(); ++s )
                {
                    output += "s" ;
                    output += boost::lexical_cast<std::string>
                              ( service.deps[s] ) ;
                    output += "   ";
                }
                if (  service.deps.size() == 0 ) output += "-";
                output += "\n";
                
                return output ;                
            }
            
            /**
             * Says if the given service exists or not.
             * 
             * @param serviceId Service's id.
             * @return True if the service exists, false otherwise.
             */ 
            bool
            exists ( uint serviceId ) const
            {
                if ( serviceId >= size() )
                {
                    std::cerr
                        << std::string ( "Service with id=" )
                        << boost::lexical_cast<std::string>( serviceId )
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
            
            /**
             * Returns the service identified by serviceId.
             * 
             * @param serviceId the id of the service you are looking for.
             * @return Service.
             */ 
            boost::shared_ptr<const Service>
            get ( ushort serviceId ) const
            {
                assert ( serviceId < size() );
                
                return _services [ serviceId ];
            }
            ///@}
            
        public:
            
            /**
             * @name Service's index management
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
                return _machines.startIndex() +
                       1 +
                       _machines.size()*_machines.getDefinitionSize();
            }
                   
            /**
             * Returns the fixed size of the machine's definition.
             * 
             * @return The number of integers it takes to define a machine.
             */
            uint
            getAllServicesDefinitionSize () const
            {
                return _definitionSize;
            }
            ///@}
            
        private:
            
            const Machines&         _machines;
            const std::vector<int>& _values;
            
            std::vector< boost::shared_ptr<const Service> > _services;
            int                                             _definitionSize;
    };
};

#endif
