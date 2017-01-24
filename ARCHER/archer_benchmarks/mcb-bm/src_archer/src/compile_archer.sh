#! /bin/bash -xe

RED='\e[0;31m'
LRED='\e[1;31m'
GREEN='\e[0;32m'
LGREEN='\e[1;32m'
NC='\e[0m'
echocg() { echo -e "${GREEN}$1${NC}${LGREEN}$2${NC}"; }
echocr() { echo -e "${RED}$1${NC}${LRED}$2${NC}"; }

export LLVM_PATH=$HOME/usr/bin
export CLANG=$LLVM_PATH/clang
export OPT=$LLVM_PATH/opt
export LLC=$LLVM_PATH/llc
export LLVM_DIS=$LLVM_PATH/llvm-dis
export LLVM_LINK=$LLVM_PATH/llvm-link

EXT=cc

PASS=$HOME/compilers/llvm35omp/Archer/lib/libArcher.so
PROG_NAME=$1
BC_FILES=

INCLUDE+=$(for i in $(find . -type d); do echo "-I"${i%%/}; done)

rm -rf .blacklists
rm -rf */.blacklists

make -f Makefile-linux-x86_64

BC_FILES=
for i in `find . -name "*.bc" | grep -v "\.nomp\.bc"`
do
    BC_FILES+=$i" "
done

# Polly Pass
for i in `find . -name "*.nomp.bc"`
do
    $OPT -mem2reg $i -o $i.opt
    $OPT -S -polly-canonicalize $i.opt > $i.ll
    $OPT -load $PASS -archer-dda $i.ll -o $i-optmized.ll
done

# Plugin to find OpenMP constructs information
for i in `find . -name "*.$EXT" | grep -v timing`
do
    mpicc -cc=clang -c -fopenmp -O0 -g -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS \
          -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D_GNU_SOURCE \
	  -DORTHOXY -DUSE_OPENMP -DUSE_MPI -DNOASSERT \
	  -I$INCLUDE -I. \
	  -I$HOME/usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include \
          $i -fsyntax-only \
          -Xclang -load -Xclang ArcherPlugin.so -Xclang \
          -plugin -Xclang archer
done

# Instruction Context Pass
$LLVM_LINK -o $PROG_NAME.bc $BC_FILES
$LLVM_DIS -f $PROG_NAME.bc -o $PROG_NAME.ll
$OPT -load  $PASS -archer-instcontext $PROG_NAME.ll -o $PROG_NAME.bc

rm -rf *.bc *.ll *.s *.opt .blacklists

for i in `find . -name "*.$EXT"`
do
    archer-bl $i $2
done

mkdir -p .blacklists
make -f Makefile-linux-x86_64
