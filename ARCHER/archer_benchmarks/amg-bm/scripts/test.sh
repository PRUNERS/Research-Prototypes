#!/bin/bash

function run {
    if [[ $host == "cab"* ]]
    then
	msub $pwd/moabconf
    else
	$pwd/job.sh
    fi
}

# export host="none"
export host=$HOSTNAME
export size=20
export param="-pooldist 0 -r $size $size $size"
header="#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)"

export pwd=$(pwd)
for j in 1 2 3 4 5 6 7 8 9 10
do
    mkdir -p $pwd/../reports/reports_$j/output
    mkdir -p $pwd/../reports/reports_$j/plots
done

echo "Running AMG Release mode..."
export label=amg.release
export cmd="./amg2013 $param"
export tsanoption=""
export current=$pwd/../src/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Inspector mode..."
export label=amg.inspectordefault
export cmd="inspxe-cl-2016 -r $label -collect ti3 -- ./amg2013 $param"
export tsanoption=""
export current=$pwd/../src_inspector/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Inspector mode (knob: scope=extreme)..."
export label=amg.inspectorscope
export cmd="inspxe-cl-2016 -r $label -collect ti3 -knob scope=extreme -- ./amg2013 $param"
export tsanoption=""
export current=$pwd/../src_inspector/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Inspector mode (knob: use-maximum-resources=true)..."
export label=amg.inspectormaxres
export cmd="inspxe-cl-2016 -r $label -collect ti3 -knob use-maximum-resources=true -- ./amg2013 $param"
export tsanoption=""
export current=$pwd/../src_inspector/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

# echo "Running AMG vanilla TSan mode..."
# export label=amg.vanillatsan
# export cmd="./amg2013 $param"
# export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
# export current=$pwd/../src_tsan_orig/test
# for j in 1 2 3 4 5 6 7 8 9 10
# do
#     echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
# done
# run

echo "Running AMG TSan-Archer mode..."
export label=amg.archertsan
export cmd="./amg2013 $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
export current=$pwd/../src_tsan/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Archer mode..."
export label=amg.archer
export cmd="./amg2013 $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
export current=$pwd/../src_archer/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Archer mode (only sequential blacklisting)..."
export label=amg.sequential
export cmd="./amg2013 $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
export current=$pwd/../src_sequential/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run

echo "Running AMG Archer mode (only parallel blacklisting)..."
export label=amg.parallel
export cmd="./amg2013 $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
export current=$pwd/../src_parallel/test
for j in 1 2 3 4 5 6 7 8 9 10
do
    echo -e $header > $pwd/../reports/reports_$j/plots/$label.data
done
run
