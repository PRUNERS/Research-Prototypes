#!/bin/bash

function run {
    if [[ $host == "cab"* ]]
    then
	# msub $pwd/moabconf
	$pwd/jobs.sh "$1" "$2" "$3" "$4" "$5" "$6"
    else
	$pwd/job.sh &
    fi
}

# HALF=$1
HALF=1

# export host="none"
export host=$HOSTNAME
header="#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)"

export pwd=$(pwd)
for j in 1 2 3 4 5 6 7 8 9 10
do
    mkdir -p $pwd/../reports/reports_$j/output
    mkdir -p $pwd/../reports/reports_$j/plots
done

CMD=(
    "./c_fft.par 32768"
    "./c_fft6.par 1024 200"
    "./c_jacobi01.par 16000 16000 0.8 1.0 1e-9 100"
    "./c_jacobi02.par 16000 16000 0.8 1.0 1e-9 100"
    "./c_loopA.badSolution.par 10000000 100"
    "./c_loopA.solution1.par 200000 1000"
    "./c_loopA.solution2.par 10000000 100"
    "./c_loopA.solution3.par 1000000 1000"
    "./c_loopB.badSolution1.par 1000000 500"
    "./c_loopB.badSolution2.par 50000 100"
    "./c_loopB.pipelineSolution.par 10000000 100"
    "./c_lu.par 3000"
    "./c_mandel.par 150000"
    "./c_md.par 4096 100"
    "./c_pi.par 100000000000"
    "./c_qsort.par 5000000"
    "./c_testPath.par 1 1234 ../applications/c_GraphSearch/GraphExamples/exampleGraph_02.gph" # to fix
    "./cpp_qsomp1.par 25000000"
    "./cpp_qsomp2.par 25000000"
    "./cpp_qsomp5.par 10000000"
    "./cpp_qsomp6.par 20000000"
    "./cpp_qsomp7.par 20000000"
)

EXEC=( 
    "c_fft.par"
    "c_fft6.par"
    "c_jacobi01.par"
    "c_jacobi02.par"
    "c_loopA.badSolution.par"
    "c_loopA.solution1.par"
    "c_loopA.solution2.par"
    "c_loopA.solution3.par"
    "c_loopB.badSolution1.par"
    "c_loopB.badSolution2.par"
    "c_loopB.pipelineSolution.par"
    "c_lu.par"
    "c_mandel.par"
    "c_md.par"
    "c_pi.par"
    "c_qsort.par"
    "c_testPath.par"
    "cpp_qsomp1.par"
    "cpp_qsomp2.par"
    "cpp_qsomp5.par"
    "cpp_qsomp6.par"
    "cpp_qsomp7.par"
)

j=0;
for i in "${EXEC[@]}"
do
    export FLAG=0
    comm=${CMD[$j]}

    if [  $j -gt 16 ];
    then
    	export FLAG=1
    fi

    echo "Running $i Release mode..."
    export label=$i.release
    export cmd=$comm
    export tsanoption=""
    export current=$pwd/../src/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    echo "Running $i Inspector mode..."
    export label=$i.inspectordefault
    export cmd="inspxe-cl-2016 -r $label -collect ti3 -- $comm"
    export tsanoption=""
    export current=$pwd/../src_inspector/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    echo "Running $i Inspector mode (knob: scope=extreme)..."
    export label=$i.inspectorscope
    export cmd="inspxe-cl-2016 -r $label -collect ti3 -knob scope=extreme -- $comm"
    export tsanoption=""
    export current=$pwd/../src_inspector/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"
    
    echo "Running $i Inspector mode (knob: use-maximum-resources=true)..."
    export label=$i.inspectormaxres
    export cmd="inspxe-cl-2016 -r $label -collect ti3 -knob use-maximum-resources=true -- $comm"
    export tsanoption=""
    export current=$pwd/../src_inspector/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    # echo "Running $i vanilla TSan mode..."
    # export label=$i.vanillatsan
    # export cmd=$comm
    # export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
    # export current=$pwd/../src_tsan_orig/bin
    # for h in 1 2 3 4 5 6 7 8 9 10
    # do
    #     echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    # done
    # run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"
    
    echo "Running $i TSan-Archer mode..."
    export label=$i.archertsan
    export cmd=$comm
    export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
    export current=$pwd/../src_tsan/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    echo "Running $i Archer mode..."
    export label=$i.archer
    export cmd=$comm
    export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
    export current=$pwd/../src_archer/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    echo "Running $i Archer mode (only sequential blacklisting)..."
    export label=$i.sequential
    export cmd=$comm
    export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
    export current=$pwd/../src_sequential/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    echo "Running $i Archer mode (only parallel blacklisting)..."
    export label=$i.parallel
    export cmd=$comm
    export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0\""
    export current=$pwd/../src_parallel/bin
    for h in 1 2 3 4 5 6 7 8 9 10
    do
        echo -e $header > $pwd/../reports/reports_$h/plots/$label.data
    done
    run "$current" "$cmd" "$label" "$tsan_option" "$FLAG" "$HALF"

    ((j++))
done
