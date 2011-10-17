#!/usr/bin/env ruby
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

# Calculates the mean of values delimited by <left> in the left
# and <right> in the right. Both keywords don't have to span the
# whole line, partial matching is fine. Pass "" as right keyword
# if you want to match the end of line. all_metrics.sh contains
# examples.
# Utilisation example:
#
# > for i in $(seq 1 10); do make a1-1; done | ../../scripts/mean.rb \
# > endcandidates= ms
#
# Or for all the a1 instances:
#
# > for j in $(seq 1 5); do for i in $(seq 1 10); do make a1-$j; \
# > done | ../../scripts/mean.rb endcandidates= ms ; done
#

if ARGV.length != 2
  
  puts "Calculates the mean of the values delimited by <left> and <right> keywords"
  puts "usage: ./mean.rb <left> <right>"

  exit 0

end

left  = ARGV[0]
right = ARGV[1]

times = []

STDIN.read.split("\n").each do |line|


  if line =~ /#{Regexp.escape(left)}(.*)#{Regexp.escape(right)}/
     times << $1.to_i 
  end

end

print (times.inject(0.0) { |sum, elem| sum + elem } / times.size).to_i.to_s

