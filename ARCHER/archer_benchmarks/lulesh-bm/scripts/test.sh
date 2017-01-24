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
export param="-s $size"
header="#threads\ttime(ms)\tmem(KB)\tmem(MB)\tmem(GB)"

export pwd=$(pwd)
mkdir -p $pwd/../reports/output
mkdir -p $pwd/../reports/plots

echo "Running Lulesh Release mode..."
export label=lulesh.release
export cmd="./lulesh.release $param"
export tsanoption=""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Inspector mode..."
export label=lulesh.inspectordefault
export cmd="inspxe-cl-2015 -r $label -collect ti3 -- ./lulesh.release $param"
export tsanoption=""
export current=$pwd/../src_inspector
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Inspector mode (knob: scope=extreme)..."
export label=lulesh.inspectorscope
export cmd="inspxe-cl-2015 -r $label -collect ti3 -knob scope=extreme -- ./lulesh.release $param"
export tsanoption=""
export current=$pwd/../src_inspector
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Inspector mode (knob: use-maximum-resources=true)..."
export label=lulesh.inspectormaxres
export cmd="inspxe-cl-2015 -r $label -collect ti3 -knob use-maximum-resources=true -- ./lulesh.release $param"
export tsanoption=""
export current=$pwd/../src_inspector
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh vanilla TSan mode..."
export label=lulesh.vanillatsan
export cmd="./lulesh.vanillatsan $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0 external_symbolizer_path=$HOME/usr/bin/llvm-symbolizer\""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh TSan-Archer mode..."
export label=lulesh.archertsan
export cmd="./lulesh.archertsan $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0 external_symbolizer_path=$HOME/usr/bin/llvm-symbolizer\""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Archer mode..."
export label=lulesh.archer
export cmd="./lulesh.archer $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0 external_symbolizer_path=$HOME/usr/bin/llvm-symbolizer\""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Archer mode (only sequential blacklisting)..."
export label=lulesh.sequential
export cmd="./lulesh.sequential $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0 external_symbolizer_path=$HOME/usr/bin/llvm-symbolizer\""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run

echo "Running Lulesh Archer mode (only parallel blacklisting)..."
export label=lulesh.parallel
export cmd="./lulesh.parallel $param"
export tsanoption="TSAN_OPTIONS=\"report_thread_leaks=0 external_symbolizer_path=$HOME/usr/bin/llvm-symbolizer\""
export current=$pwd/../src
echo -e $header > $pwd/../reports/plots/$label.data
run
