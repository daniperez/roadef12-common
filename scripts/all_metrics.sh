#!/bin/bash

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
