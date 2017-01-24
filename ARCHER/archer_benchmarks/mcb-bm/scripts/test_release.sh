#!/bin/bash
#set -x

export pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

export current=$pwd/../src/src

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.release.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_release
else
    $pwd/job_release.sh
fi
