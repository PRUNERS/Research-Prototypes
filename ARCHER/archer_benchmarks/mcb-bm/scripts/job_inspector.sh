#!/bin/bash

#set -x

if [[ $HOSTNAME == "cab"* ]]
then
    . /usr/local/tools/dotkit/init.sh
    use mvapich2-gnu-1.9
fi

pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

current=$pwd/../src/src
cd $current

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.insp.data

for j in 1 2 4 8 12 16
do
t=`expr $j - 1`
command="taskset -c 0-$t inspxe-cl-2015 -r inspector$j -collect ti3 -- ./MCBenchmark.exe $param"
echo "Running with $j threads..."
export OMP_NUM_THREADS=$j

#S="$(($(date +%s%N)/1000000))"
stantime "$command" "mcb_inspector" 1>>$pwd/../reports/output/mcb_inspector.$j.out 2>>$pwd/../reports/output/mcb_inspector.$j.out
#E="$(($(date +%s%N)/1000000))"

kmem=$(cat $current/tmp_mcb_inspector | grep "KB" | awk -F" " '{ print $2} ')
mmem=$(cat $current/tmp_mcb_inspector | grep "MB" | awk -F" " '{ print $2} ')
gmem=$(cat $current/tmp_mcb_inspector | grep "GB" | awk -F" " '{ print $2} ')
time=$(cat $current/tmp_mcb_inspector | grep "Real" | awk -F" " '{ print $2} ')
rm $current/tmp_mcb_inspector

echo -e "$j\t$time\t$kmem\t$mmem\t$gmem" >> $pwd/../reports/plots/mcb.insp.data

done
