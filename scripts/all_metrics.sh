#!/bin/bash
# The MIT License
#
# Copyright (c) 2011 daniperez
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

instances="a1-1 a1-2 a1-3 a1-4 a1-5"

# Calculates the average time of building the candidate list
echo "Time of building candidate list:"
for instance in $instances 
do
  echo -n "    $instance="
  for i in $(seq 1 10);
  do
    make $instance;
  done | ../../scripts/mean.rb endcandidates= ms
  echo "ms"
done

# Number of candidates
echo "Number of candidates:"
for instance in $instances 
do
  echo -n "    $instance="
  # Doing the mean of just 1 occurrence
  make $instance | ../../scripts/mean.rb "Number of candidates=" ""
  echo ""
done
