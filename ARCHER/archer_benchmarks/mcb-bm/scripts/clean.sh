#!/bin/bash

pwd=$(pwd)

cd $pwd/../src/src
make -f Makefile-linux-x86_64 clean
make -f Makefile-linux-x86_64 veryclean
cd $pwd/../src_tsan/src
make -f Makefile-linux-x86_64 clean
make -f Makefile-linux-x86_64 veryclean
cd $pwd/../src_archer/src
make -f Makefile-linux-x86_64 clean
make -f Makefile-linux-x86_64 veryclean
