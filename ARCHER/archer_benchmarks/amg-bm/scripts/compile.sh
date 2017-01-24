#!/bin/bash

pwd=$PWD

./clean.sh
cd $pwd/../src
make veryclean all
cd $pwd/../src_inspector
make veryclean all
cd $pwd/../src_tsan
make veryclean all
cd $pwd/../src_tsan_orig
make veryclean all
cd $pwd/../src_archer
make veryclean all
cd $pwd/../src_sequential
make veryclean all
cd $pwd/../src_parallel
make veryclean all
