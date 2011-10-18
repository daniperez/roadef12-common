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
#ifndef __roadef11_RESOURCES_HPP
#define __roadef11_RESOURCES_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11
///////////////////////////////////////////////////////////////////////////
// STD
#include <vector>
#include <stdexcept>
#include <iostream>
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF11
{    
    /**
     * Resource-related procedures.
     *
     * @author daniperez
     */
    class Resources
    {
        public:
            
            /**
             * Constructor.
             */
            Resources ( const std::vector<int>& values )
                : _values(values)
            {
            }
            
            /**
             * Number of resources.
             * 
             * @return number of resources.
             */
            uint size () const
            {
                return _values [ startIndex() ];
            }

        public:
            
            /**
             * @name Methods related to a single resource
             */
            ///@{                
            /**
             * Says if the given resource is transient or not.
             * 
             * @param resourceId Resource's id.
             * @return True if the resource is transient, false
             *         otherwise.
             */
            bool isTransient ( uint resourceId ) const
            {
                assert ( exists ( resourceId ) );
                
                return _values[ startIndex() + 1 + resourceId*getDefinitionSize() ];
            }
           
            /**
             * Returns a human-readable representation of a resource.
             * 
             * @param resourceId Resource's id.
             * @return Resource's representation for debugging purposes.
             */
            std::string toString ( uint resourceId ) const
            {
                assert ( exists ( resourceId ) );
                
                std::string output;
                                
                output += "    #" ;
                output += boost::lexical_cast<std::string> ( resourceId ) ;
                output += "                  : ";
                output += isTransient ( resourceId ) ? "tr , " : "ntr, " ;
                output += "\n";
                
                return output;                
            }
            
            /**
             * Says if the given resource exists or not.
             * 
             * @param resourceId Resource's id.
             * @return True if the resource exists, false otherwise.
             */ 
            bool exists ( uint resourceId ) const
            {
                if ( resourceId >= size() )
                {
                    std::cerr
                        << std::string ( "Resource with id=" )
                        << boost::lexical_cast<std::string>( resourceId )
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
             * @name Resource's index management
             */
            ///@{ 
            /**
             * According to ROADEF's format, where
             * this item's definition start.
             * 
             * @return Index of the start of definition.
             */ 
            uint startIndex () const
            {
                return 0;
            }
               
            /**
             * Returns the fixed size of the resource's definition.
             * 
             * @return The number of integers it takes to define a resource.
             */
            uint getDefinitionSize () const
            {
                return 2;
            }
            
            ///@}
               
        private:
            
            const std::vector<int>& _values;
    
    };
};

#endif
