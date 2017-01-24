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

MOD    = pbdriver.so \
	 pb_datatype1.so pb_datatype2.so \
	 pb_twomsg1.so pb_twomsg2.so \
	 pb_pack1.so pb_pack2.so\
         pb_copy1.so pb_copy2.so
HEADER = pb_mod.h 

CFLAGS += -I$(PNMPI_INC_PATH) -fPIC
CCFLAGS += -I$(PNMPI_INC_PATH) -fPIC

all: $(MOD) install

pb_dt.o: 

.SUFFIXES: .c .o .so

.o.so: 
	$(CXX) -o $@ $(SFLAGS) $<

.c.o:
	$(MPICC) -c $(CFLAGS) $<

.cpp.o:
	$(MPIXX) -c $(CCFLAGS) $<

.cxx.o:
	$(MPIXX) -c $(CCFLAGS) $<

pb_datatype1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_DATATYPE1 $<
	mv piggyback.o pb_datatype1.o

pb_datatype2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_DATATYPE2 $<
	mv piggyback.o pb_datatype2.o

pb_twomsg1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_TWOMSG1 $<
	mv piggyback.o pb_twomsg1.o

pb_twomsg2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_TWOMSG2 $<
	mv piggyback.o pb_twomsg2.o

pb_pack1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_PACK1 $<
	mv piggyback.o pb_pack1.o

pb_pack2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_PACK2 $<
	mv piggyback.o pb_pack2.o

pb_copy1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_COPY1 $<
	mv piggyback.o pb_copy1.o

pb_copy2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_COPY2 $<
	mv piggyback.o pb_copy2.o


install: $(MOD)
	for mymod in $(MOD); do ($(PNMPIBASE)/patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

