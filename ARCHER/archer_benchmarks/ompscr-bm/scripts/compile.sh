#!/bin/bash -x

#./clean.sh
pwd=$(pwd)

cd $pwd/../src
mkdir -p bin log
gmake distclean par
cd $pwd/../src_inspector
mkdir -p bin log
gmake distclean par
cd $pwd/../src_tsan_orig
mkdir -p bin log
gmake distclean par
cd $pwd/../src_tsan
mkdir -p bin log
gmake distclean par
cd $pwd/../src_archer
mkdir -p bin log
gmake distclean par
cd $pwd/../src_sequential
mkdir -p bin log
gmake distclean par
cd $pwd/../src_parallel
mkdir -p bin log
gmake distclean par
