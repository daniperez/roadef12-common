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
#ifndef __roadef12_FILE_PARSER_HPP
#define __roadef12_FILE_PARSER_HPP
///////////////////////////////////////////////////////////////////////////
// roadef12
#include "roadef12-common/util/Util.hpp"
#include "roadef12-common/service/ServiceExceptions.hpp"
#include "roadef12-common/objects/Assignment.hpp"
///////////////////////////////////////////////////////////////////////////
// STD
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <list>
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/shared_ptr.hpp>
///////////////////////////////////////////////////////////////////////////

namespace ROADEF12COMMON {

    /**
     * File parser.
     *
     * @author daniperez
     */
    class FileParser
    {   
        public:
            
            /**
             * Parses a file and puts the content into a vector.
             * 
             * @param fileName File to parse.
             * @param output vector to fill in.
             * @throw IOException if the input files cannot be read.
             * @throw ParseException if the input files have wrong format.
             */
            static void
            parseVector ( const char* fileName, std::vector<int>& output )
            throw ( ROADEF12COMMON::IOException, ROADEF12COMMON::ParseException )
            {
                throwing_assert ( fileName != NULL );
                
                std::ifstream file ( fileName, std::ios::in );
                
                if ( file.fail() )
                {
                    throw ROADEF12COMMON::IOException
                    (
                        (
                            std::string ( "'" ) + fileName +
                            std::string ( "' cannot be open" )
                        ).c_str()
                    );
                }

                int value = 0;
                
                while ( file >> value )
                {
                    output.push_back ( value );
                    
                    if ( file.fail() )
                    {
                        throw ROADEF12COMMON::IOException
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
                    throw ROADEF12COMMON::IOException
                    (
                        (std::string(fileName) +
                            std::string (" cannot be closed")).c_str()
                    );
                }
            }
    };
};

#endif
