#!/bin/bash

./clean.sh
export SIZE=20

pwd=$(pwd)

cd $pwd/../src
make clean all
make -f Makefile.tsan clean all
./compile_archer.sh lulesh.archer 0
./compile_archer.sh lulesh.sequential 1
./compile_archer.sh lulesh.parallel 2
cd $pwd/../src_inspector
make clean all
