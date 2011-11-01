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
#ifndef __roadef12_common_UTIL_HPP
#define __roadef12_common_UTIL_HPP
///////////////////////////////////////////////////////////////////////////
// roadef12
#include "roadef12-common/service/ServiceExceptions.hpp"
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/lexical_cast.hpp>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////
// STD
#include <iostream>
#include <time.h>
#include <unordered_map>
///////////////////////////////////////////////////////////////////////////

/**
 * Like assert(3) but throws an std::logic_error instead.
 * Prints a stack trace in linux.
 * 
 * @param cond If cond is false, the exception is thrown.
 * @param fileName File where the assert occurs.
 * @param lineNumber Line number.
 * @throw Throws std::logic_error if cond is false.
 */
#define throwing_assert(x) __throwing_assert(x,__FILE__,__LINE__)
void __throwing_assert ( bool cond, const char* fileName, int lineNumber )
{
    
    #if DEBUG
    if ( ! cond )
    {
        std::string msg = "assert failed (" ;
        msg            += fileName ;
        msg            += ":" ;
        msg            += boost::lexical_cast<std::string> ( lineNumber ) ;
        msg            += ")" ;
        
        throw std::logic_error ( msg.c_str() );
    }
    #endif
}

#endif
