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
#ifndef __roadef11_MODEL_HPP
#define __roadef11_MODEL_HPP
///////////////////////////////////////////////////////////////////////////
// roadef11 Command
#include "roadef11-common/objects/model/Machines.hpp"
#include "roadef11-common/objects/model/Resources.hpp"
#include "roadef11-common/objects/model/Services.hpp"
#include "roadef11-common/objects/model/Processes.hpp"
#include "roadef11-common/objects/model/GeneralCosts.hpp"
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
     * Input parameters.
     * 
     * @author daniperez
     */
    class Parameters
    {
        public:
            
            /**
             * Constructor.
             */
            Parameters ( const std::vector<int>& parameters )
                : resources ( parameters ),
                  machines  ( resources, parameters ),
                  services  ( machines, parameters ),
                  processes ( resources, services, parameters ),
                  costs     ( processes, resources, parameters )
            {
            }
            
        public:

            /**
             * Resources.
             */
            const Resources    resources;
            
            /**
             * Machines.
             */
            const Machines     machines;
            
            /**
             * Services.
             */
            const Services     services;
            
            /**
             * Processes.
             */
            const Processes    processes;
            
            /**
             * Costs.
             */
            const GeneralCosts costs;
    };

};

#endif
