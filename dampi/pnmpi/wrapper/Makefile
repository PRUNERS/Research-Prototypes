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

include $(PROJECTS)/common/Makefile.common

MPISPEC = mpi_pnmpi
#MPISPEC = mpi_def

PROTOFILE = $(MPISPEC)_proto
FCTFILE   = $(MPISPEC)_fct

WRAPPERC = wrapper_c.w
WRAPPERH = wrapper_h.w
NEWSTACK = newstack_h.w

WR = ../wrappergen/wrappergen 

TARGET = wrapper.c wrapper.h newstack.h wrapper_f77.c f77symbols.h

all: $(TARGET)

dummy.fct: dummy.c
	$(MPICC) -E dummy.c | gawk -f join_lines | grep -e "PMPI_" | grep -v "_FN" | grep -v f2c | grep -v c2f | sed 's/(/ (/' | grep -v PMPI_Pcontrol | grep -v PMPI_Type_count > dummy.fct
	# $(MPICC) -E dummy.c | sed 's/__attribute__[(][(]visibility[(]\"default\"[)][)][)][[:space:]]//' | gawk -f join_lines | sed -e 's/  */ /g' | grep -e "PMPI_" | grep -v "_FN" | grep -v f2c | grep -v c2f | sed 's/(/ (/' | grep -v PMPI_Pcontrol | grep -v PMPI_Type_count > dummy.fct

wrapper.c: $(PROTOFILE) $(FCTFILE) $(WRAPPERC)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(WRAPPERC) -o wrapper.c

wrapper.h: $(PROTOFILE) $(FCTFILE) $(WRAPPERH)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(WRAPPERH) -o wrapper.h

newstack.h: $(PROTOFILE) $(FCTFILE) $(NEWSTACK)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(NEWSTACK) -o newstack.h
	cp newstack.h ../src/
	cp newstack.h ../include/

wrapper_f77.c f77_symbol.h: mpi-f77.txt makefortran.py
	python makefortran.py --xlate --f77symbol $(F77SYMBOL) mpi-f77.txt

mpi_pnmpi_fct-nopctrl: dummy.fct
	cat dummy.fct | gawk -f get_fct | sed 's/PMPI/MPI/' | sort | uniq > mpi_pnmpi_fct-nopctrl

mpi_pnmpi_fct: mpi_pnmpi_fct-nopctrl
	gawk 'BEGIN{print "MPI_Pcontrol"}{print}'< mpi_pnmpi_fct-nopctrl > mpi_pnmpi_fct

mpi_pnmpi_proto: dummy.fct mpi_pnmpi_fct-nopctrl extrargs
	./generate_protp_file.sh mpi_pnmpi_fct-nopctrl dummy.fct | gawk -f add-pctrl> mpi_pnmpi_proto

extrargs: extrargs.o
	$(CC) -o $@ -g $<

extrargs.o: extrargs.c
	$(CC) -c $< -g

clean:
	rm -f dummy.fct $(PROTOFILE) $(FCTFILE) $(TARGET) extrargs *.o lookup.c mpiPi_def.h

clobber: clean
	rm -f *~


