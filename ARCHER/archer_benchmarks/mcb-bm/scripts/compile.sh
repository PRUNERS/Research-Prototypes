#!/bin/bash

if [[ $HOSTNAME == "cab"* ]]
then
    . /usr/local/tools/dotkit/init.sh
    use mvapich2-gnu-1.9
fi

pwd=$PWD

cd $pwd/../src/src
make -f Makefile-linux-x86_64 veryclean all
cd $pwd/../src_tsan/src
make -f Makefile-linux-x86_64 veryclean all
cd $pwd/../src_archer/src
make -f Makefile-linux-x86_64 veryclean
./compile_archer.sh MCBenchmark.exe 0
mv MCBenchmark.exe MCBenchmark.exe.archer
make -f Makefile-linux-x86_64 veryclean
./compile_archer.sh MCBenchmark.exe 1
mv MCBenchmark.exe MCBenchmark.exe.sequ
make -f Makefile-linux-x86_64 veryclean
./compile_archer.sh MCBenchmark.exe 2
mv MCBenchmark.exe MCBenchmark.exe.para
