#! /bin/bash -e

if [  "$#" -lt 2 ]; then
    echo "Usage: compile_archer.sh EXECUTABLE_NAME OPTION[0=Blacklist all,1=Only Sequential instructions,2=Only Parallel Instruction from DDA]"
    exit 1
fi

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
mkdir -p .blacklists

# Polly Pass
for i in `find . -name "*.$EXT"`
do
    mpicc -cc=$CLANG -DUSE_MPI=0 -DSIZE=$SIZE -I$HOME/usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include -I. -emit-llvm -O0 -g -c $i -o $i.bc
    $OPT -mem2reg $i.bc -o $i.opt
    mv $i.opt $i.bc
    $OPT -S -polly-canonicalize $i.bc > $i.ll
    $OPT -load $PASS -archer-dda $i.ll -o $i-optmized.ll
done

# Plugin to find OpenMP constructs information
for i in `find . -name "*.$EXT"`
do
    clang -fopenmp -O0 -g -DUSE_MPI=0 -DSIZE=$SIZE -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS \
        -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D_GNU_SOURCE \
	-I$HOME/usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include \
        $i -fsyntax-only \
        -Xclang -load -Xclang ArcherPlugin.so -Xclang \
        -plugin -Xclang archer
done

# Instruction Context Pass
for i in `find . -name "*.$EXT"`
do
    mpicc -cc=$CLANG -fopenmp -DUSE_MPI=0 -DSIZE=$SIZE -I$HOME/usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include -I. -emit-llvm -O0 -g -c $i -o $i.bc
    BC_FILES+=$i.bc" "
done

$LLVM_LINK -o $PROG_NAME.bc $BC_FILES
$LLVM_DIS -f $PROG_NAME.bc -o $PROG_NAME.ll
$OPT -load  $PASS -archer-instcontext $PROG_NAME.ll -o $PROG_NAME.bc

rm -rf *.bc *.ll *.s

for i in `find . -name "*.$EXT"`
do
    archer-bl $i $2
    mpicc -cc=clang -DUSE_MPI=0 -DSIZE=$SIZE -I$HOME/usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include -g -O0 -fopenmp -fsanitize=thread -fsanitize-blacklist=.blacklists/$i.bl -c -o $i.o $i
    OBJ_FILES+=$i.o" "
done

mpicc -cc=clang $OBJ_FILES -lm -g -O0 -pie -fopenmp -fsanitize=thread -L$HOME/usr/lib/intelomprt -liomp5_tsan -lstdc++ -o $PROG_NAME
