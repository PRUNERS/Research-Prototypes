#!/bin/bash
#set -x

export pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

export current=$pwd/../src_tsan/src

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.tsan.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_tsan
else
    $pwd/job_tsan.sh
fi
