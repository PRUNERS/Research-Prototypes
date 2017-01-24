#!/bin/bash
#set -x

export pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

export current=$pwd/../src_archer/src

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.archer.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_archer
else
    $pwd/job_archer.sh
fi

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.sequ.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_sequential
else
    $pwd/job_sequ.sh
fi

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.para.data

if [[ $HOSTNAME == "cab"* ]]
then
    msub $pwd/mcb_parallel
else
    $pwd/job_para.sh
fi
