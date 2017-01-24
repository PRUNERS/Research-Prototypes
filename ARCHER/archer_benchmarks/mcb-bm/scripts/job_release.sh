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

echo -e "#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)" > $pwd/../reports/plots/mcb.release.data

for j in 1 2 4 8 12 16
do
t=`expr $j - 1`
command="taskset -c 0-$t ./MCBenchmark.exe $param"
echo "Running with $j threads..."
export OMP_NUM_THREADS=$j

#S="$(($(date +%s%N)/1000000))"
stantime "$command" "mcb_release" 1>>$pwd/../reports/output/mcb_release.$j.out 2>>$pwd/../reports/output/mcb_release.$j.out
#E="$(($(date +%s%N)/1000000))"

kmem=$(cat $current/tmp_mcb_release | grep "KB" | awk -F" " '{ print $2} ')
mmem=$(cat $current/tmp_mcb_release | grep "MB" | awk -F" " '{ print $2} ')
gmem=$(cat $current/tmp_mcb_release | grep "GB" | awk -F" " '{ print $2} ')
time=$(cat $current/tmp_mcb_release | grep "Real" | awk -F" " '{ print $2} ')
rm $current/tmp_mcb_release

echo -e "$j\t$time\t$kmem\t$mmem\t$gmem" >> $pwd/../reports/plots/mcb.release.data

done
