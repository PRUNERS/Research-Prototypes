#!/bin/bash

# ulimit -s 8192

if [[ $HOSTNAME == "cab"* ]]
then
    . /usr/local/tools/dotkit/init.sh
    use mvapich2-gnu-1.9
fi

cd $current
for j in 1 2 4 8 12 16
do
    t=`expr $j - 1`
    command="taskset -c 0-$t ./MCBenchmark.exe.para $param"
    echo "Running with $j threads..."
    export OMP_NUM_THREADS=$j
    
    stantime "$command" "mcb_para" 1>>$pwd/../reports/output/mcb_para.$j.out 2>>$pwd/../reports/output/mcb_para.$j.out
    
    kmem=$(cat $current/tmp_mcb_para | grep "KB" | awk -F" " '{ print $2} ')
    mmem=$(cat $current/tmp_mcb_para | grep "MB" | awk -F" " '{ print $2} ')
    gmem=$(cat $current/tmp_mcb_para | grep "GB" | awk -F" " '{ print $2} ')
    time=$(cat $current/tmp_mcb_para | grep "Real" | awk -F" " '{ print $2} ')
    rm $current/tmp_mcb_para
    
    echo -e "$j\t$time\t$kmem\t$mmem\t$gmem" >> $pwd/../reports/plots/mcb.para.data
done
