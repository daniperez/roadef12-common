#!/usr/bin/env ruby
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

