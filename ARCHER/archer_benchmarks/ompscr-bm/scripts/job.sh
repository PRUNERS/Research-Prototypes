#!/bin/bash

if [[ $HOSTNAME == "cab"* ]]
then
    . /usr/local/tools/dotkit/init.sh
    use mvapich2-gnu-1.9
fi

LIST="1 2 3 4 5 6 7 8 9 10"
# LIST="1 2 3 4 5"
# if [[ $HALF -eq 2 ]]
# then
#     LIST="6 7 8 9 10"
# fi

cd $current
for k in $LIST
do
    for j in 2 4 8 12 16
    do
        param=""
        t=`expr $j - 1`
        command="$tsanoption taskset -c 0-$t $cmd"
        echo "Running with $j threads..."
        export OMP_NUM_THREADS=$j
        if [ "$FLAG" -eq "1" ];
        then
	    param=" $j 10000"
        fi

        echo "$current - stantime \"$command$param\" $label$j 1>>$pwd/../reports/reports_$k/output/$label.$j.out 2>>$pwd/../reports/reports_$k/output/$label.$j.err"
        stantime "$command$param" $label$j 1>>$pwd/../reports/reports_$k/output/$label.$j.out 2>>$pwd/../reports/reports_$k/output/$label.$j.err

        kmem=$(cat $current/tmp_$label$j | grep "KB" | awk -F" " '{ print $2} ')
        mmem=$(cat $current/tmp_$label$j | grep "MB" | awk -F" " '{ print $2} ')
        gmem=$(cat $current/tmp_$label$j | grep "GB" | awk -F" " '{ print $2} ')
        time=$(cat $current/tmp_$label$j | grep "Real" | awk -F" " '{ print $2} ')
        rm $current/tmp_$label$j

        echo -e "$j\t$time\t$kmem\t$mmem\t$gmem" >> $pwd/../reports/reports_$k/plots/$label.data

        if [ -d "$label" ]; then
    	    mv $label $label$j
        fi
    done

    lb=$(echo $label | awk -F ".par." '{ print $2 }')
    case "$lb" in
        'inspectordefault')
            mkdir -p reports_$k
            mv $label* reports_$k
            ;;
        'inspectorscope')
            mkdir -p reports_$k
            mv $label* reports_$k
            ;;
        'inspectormaxres')
            mkdir -p reports_$k
            mv $label* reports_$k
            ;;
    esac
done
