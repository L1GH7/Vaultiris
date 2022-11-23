#!/bin/bash

STRING_LENGTHS=(100 10000 1000000)
TRIM_STRATEGIES=(old js built)
N=1000

for LENGTH in ${STRING_LENGTHS[@]}; do
  for STRATEGY in ${TRIM_STRATEGIES[@]}; do
    # Run the script in the main root
    node ./test.js $LENGTH $STRATEGY $N > results-$STRATEGY-$LENGTH.txt
  done
done
