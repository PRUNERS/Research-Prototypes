#!/bin/bash
#set -x

export pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

# One node allocation
# Maybe use expect
# salloc -n 1 -ppdebug  

export current=$pwd/../src/src

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.insp.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_inspector
else
    $pwd/job_inspector.sh
fi
