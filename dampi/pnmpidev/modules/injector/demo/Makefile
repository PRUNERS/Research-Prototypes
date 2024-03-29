# Copyright (c) 2015
# Lawrence Livermore National Security, LLC.

# Produced at the Lawrence Livermore National Laboratory.
# Written by Anh Vo, Sriram Aananthakrishnan, Ganesh Gopalakrishnan,
# Bronis R. de Supinski, Martin Schulz, and Greg Bronevetsky
# Contact email: schulzm@llnl.gov, LLNL-CODE-647203
# All rights reserved - please read information in "LICENSE".

# This file is part of DAMPI. For details see
# https://github.com/soarlab/DAMPI.

# Please also read the file "LICENSE" included in this package for Our
# Notice and GNU Lesser General Public License.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License (as published by
# the Free Software Foundation) version 2.1 dated February 1999.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
# conditions of the GNU General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the

# Free Software Foundation, Inc., 
# 59 Temple Place, Suite 330, 
# Boston, MA 02111-1307 USA

include $(PNMPIBASE)/common/Makefile.common

#TARGET = local loader
TARGET = injector-tester-asynch injector-tester-scatter injector-tester-broadcast injector-tester-complete

ifeq ($(findstring AIX,$(OS)),AIX)
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so $(PNMPI_LIB_PATH)/forcempi.a
LFLAGS += -brtl
else
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so
LFLAGS += 
endif

HERE   = $(PWD)

all: $(TARGET)

fdemo.o: fdemo.f
	$(MPIF77) -c $(FFLAGS) $<

fdemo: fdemo.o
	$(MPIF77) -o $@ $(LFLAGS) $<

fdemo-pn: $(PNMPILIB) fdemo.o
	$(MPIF77) -o $@ $(LFLAGS)  fdemo.o $(PNMPILIB) -ldl

fdemo-s1: fdemo.o
	$(MPIF77) -o $@ $(LFLAGS) $< ../modules/sample/sample1f.so 

simple: simple.o
	$(MPICC) -o $@ $(LFLAGS) $< 

simple-pn: $(PNMPILIB) simple.o
	$(MPICC) -o $@  $(LFLAGS) simple.o -L$(PNMPI_LIB_PATH) -lpnmpi

simple-s1: $(PNMPILIB) simple.o
	$(MPICC) -o $@  $(LFLAGS) simple.o ../modules/sample/sample1.so -ldl

simple.o: simple.c
	$(MPICC) -c $(CFLAGS) $<

injector-tester: injector-tester.c
	$(MPICC) -o $@  $(LFLAGS) injector-tester.c -L$(PNMPI_LIB_PATH) -lpnmpi

injector-tester-asynch: injector-tester-asynch.c
	$(MPICC) -o $@  $(LFLAGS) injector-tester-asynch.c -L$(PNMPI_LIB_PATH) -lpnmpi

injector-tester-scatter: injector-tester-scatter.c
	$(MPICC) -o $@  $(LFLAGS) injector-tester-scatter.c -L$(PNMPI_LIB_PATH) -lpnmpi

injector-tester-broadcast: injector-tester-broadcast.c
	$(MPICC) -o $@  $(LFLAGS) injector-tester-broadcast.c -L$(PNMPI_LIB_PATH) -lpnmpi

injector-tester-complete: injector-tester-complete.c
	$(MPICC) -o $@  $(LFLAGS) injector-tester-complete.c -L$(PNMPI_LIB_PATH) -lpnmpi

nocore:
	rm -f *.core

clean: nocore
	rm -f $(TARGET) $(MPIPTARGET) *.o

clobber: clean
	rm -f *~ *.?.mpiP *core
