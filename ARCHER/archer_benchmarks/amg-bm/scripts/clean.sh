#!/bin/bash

pwd=$(pwd)

cd $pwd/../src
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_inspector
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_tsan
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_tsan_orig
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_archer
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_sequential
make -f Makefile clean
make -f Makefile veryclean
cd $pwd/../src_parallel
make -f Makefile clean
make -f Makefile veryclean
