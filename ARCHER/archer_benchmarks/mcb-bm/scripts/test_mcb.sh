#!/bin/bash

export param="--nZonesX=800 --nZonesY=800 --numParticles=1000000"

echo "Running MCB release mode..."
./test_release.sh

echo "Running MCB inspector mode..."
./test_inspector.sh

echo "Running MCB TSan mode..."
./test_tsan.sh

# echo "Running MCB Archer mode..."
# ./test_archer.sh
