#!/bin/bash

export current=$1
export cmd=$2 
export label=$3 
export tsan_option=$4
export FLAG=$5
export HALF=$6

echo -e "Submitting job with the following parameters:"
echo -e "PATH:\t$current"
echo -e "COMMAND:\t$cmd"
echo -e "LABEL:\t$label" 
echo -e "TSAN_OPT:\t$tsan_option"
echo -e "FLAG:\t$FLAG"
echo -e "HALF:\t$HALF"
msub $pwd/moabconf
