SHELL = /bin/sh
.SUFFIXES: .cc .o

LULESH_EXEC = lulesh.release

MPI_INC = /opt/local/include/openmpi
MPI_LIB = /opt/local/lib

SERCXX = g++ -DUSE_MPI=0
MPICXX = mpig++ -cc=icpc-15.0.133 -DUSE_MPI=0 -DSIZE=$(SIZE)
CXX = $(MPICXX)

SOURCES2.0 = \
	lulesh.cc \
	lulesh-comm.cc \
	lulesh-viz.cc \
	lulesh-util.cc \
	lulesh-init.cc
OBJECTS2.0 = $(SOURCES2.0:.cc=.o)

CXXFLAGS = -g -O0 -openmp -fPIC -I. -Wall
LDFLAGS = -g -O0 -pie -openmp -L$(HOME)/usr/lib/intelomprt -liomp5 -lstdc++

.cc.o: lulesh.h
	@echo "Building $<"
	$(CXX) -c $(CXXFLAGS) -o $@  $<

all: $(LULESH_EXEC)

lulesh.release: $(OBJECTS2.0)
	@echo "Linking"
	$(CXX) $(OBJECTS2.0) $(LDFLAGS) -lm -o $@

clean:
	/bin/rm -f *.o *~ $(OBJECTS) $(LULESH_EXEC)
	/bin/rm -rf *.dSYM
