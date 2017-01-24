#!/bin/bash

# ulimit -s 8192

if [[ $HOSTNAME == "cab"* ]]
then
    . /usr/local/tools/dotkit/init.sh
    use mvapich2-gnu-1.9
fi

cd $current
for j in 2 4 8 12 16
do
    t=`expr $j - 1`
    command="$tsanoption taskset -c 0-$t $cmd"
    echo "Running with $j threads..."
    export OMP_NUM_THREADS=$j
    
    stantime "$command" $label$j 1>>$pwd/../reports/output/$label.$j.out 2>>$pwd/../reports/output/$label.$j.err
    
    kmem=$(cat $current/tmp_$label$j | grep "KB" | awk -F" " '{ print $2} ')
    mmem=$(cat $current/tmp_$label$j | grep "MB" | awk -F" " '{ print $2} ')
    gmem=$(cat $current/tmp_$label$j | grep "GB" | awk -F" " '{ print $2} ')
    time=$(cat $current/tmp_$label$j | grep "Real" | awk -F" " '{ print $2} ')
    rm $current/tmp_$label$j
    
    echo -e "$j\t$time\t$kmem\t$mmem\t$gmem" >> $pwd/../reports/plots/$label.data

    if [ -d "$label" ]; then
	mv $label $label$j
    fi
done
