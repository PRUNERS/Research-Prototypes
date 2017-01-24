#!/bin/bash

pwd=$(pwd)

cd $pwd/../src
make clean
make -f Makefile.tsan clean
rm -rf *.bc *.ll *.s .blacklists lulesh.archer lulesh.sequential lulesh.parallel lulesh.vanillatsan lulesh.archertsan
cd $pwd/../src_inspector
make clean