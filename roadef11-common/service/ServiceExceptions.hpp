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
#ifndef __roadef11_SERVICE_EXCEPTIONS_HPP
#define __roadef11_SERVICE_EXCEPTIONS_HPP
///////////////////////////////////////////////////////////////////////////
// STD
#include <stdexcept>
///////////////////////////////////////////////////////////////////////////
namespace ROADEF11 
{
    /**
     * When an input parameter is not valid.
     *
     * @author daniperez
     */
    class InvalidParametersException : public std::runtime_error
    {
        public:

            /**
             * Constructor.
             *
             * @param msg Message to show.
             */
            InvalidParametersException ( const char* msg )
              : std::runtime_error ( msg )
            {
            }
    };

    /**
     * Error reading/writing to any type of device.
     *
     * @author daniperez
     */
    class IOException : public std::runtime_error
    {
        public:

            /**
             * Constructor.
             *
             * @param msg Message to show.
             */
            IOException ( const char* msg )
              : std::runtime_error ( msg )
            {
            }
    };

    /**
     * If input contains errors.
     *
     * @author daniperez
     */
    class ParseException : public std::runtime_error
    {
        public:

            /**
             * Constructor.
             *
             * @param msg Message to show.
             */
            ParseException ( const char* msg )
              : std::runtime_error ( msg )
            {
            }
    };
    
    
    /**
     * If launched while command-line is processed, the optimization
     * is not launched.
     *
     * @author daniperez
     */
    struct DoNotContinue
    {
    };
    
    /**
     * Thrown by the checker if the solution is not valid.
     * 
     * @author daniperez
     */
    struct InvalidSolution
    {  
    };
};

#endif
