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

MOD    = empty.so


CFILES = $(MOD:.so=.c)
OFILES = $(MOD:.so=.o)

CFLAGS += -I../../include -fPIC

MPISPEC = mpi_pnmpi
WRAPDIR = ../../wrapper

PROTOFILE = $(WRAPDIR)/$(MPISPEC)_proto
FCTFILE   = $(WRAPDIR)/$(MPISPEC)_fct

WR = ../../wrappergen/wrappergen

TARGET = $(MOD) install-d

all: $(TARGET)


.SUFFIXES: .c .o .so .a

.o.a: 
	$(AR) $(ARFLAGS) $@ $<

.o.so: 
	$(LD) -o $@ $(SFLAGS) $< 

.c.o:
	$(MPICC) -c $(CFLAGS) $<


empty.h: empty.w
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $< -o $@

empty.c: empty.h

install-d: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod); done

clean:
	rm -f $(MOD) *.o *.a empty.h

clobber: clean
	rm -f *~
	for mymod in $(TARGET); do ( rm -f $(PNMPI_LIB_PATH)/$$mymod ); done
