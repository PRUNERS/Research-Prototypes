#!/bin/bash

pwd=$(pwd)

cd $pwd/../src
make clean
rm -rf bin log
cd $pwd/../src_inspector
make clean
rm -rf bin log
cd $pwd/../src_tsan
make clean
rm -rf bin log
cd $pwd/../src_tsan_orig
make clean
rm -rf bin log
cd $pwd/../src_archer
make clean
rm -rf bin log
cd $pwd/../src_sequential
make clean
rm -rf bin log
cd $pwd/../src_parallel
make clean
rm -rf bin log
