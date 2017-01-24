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

ifeq ($(findstring AIX,$(OS)),AIX)
TARGET = libpnmpi.so libpnmpi.a libpnmpif.so libpnmpif.a forcempi.a
else
TARGET = libpnmpi.so libpnmpi.a libpnmpif.so libpnmpif.a 
endif

OBJS   = core.o wrapper.o debug.o services.o
FOBJS   = core.o wrapperf.o debug.o services.o
INCS   = -I../include # -DNOSTATUS
DEPS   = pnmpi.h pnmpi_int.h wrapper.h Makefile

HEADER = pnmpi.h pnmpimod.h

all: $(TARGET) install

#MPICC = purify gcc-3.4.4
ifeq ($(findstring AIX,$(OS)),AIX)
CFLAGS += -fPIC -G
else
CFLAGS += -fPIC
endif

FORTRANFLAG = -DCOMPILE_FOR_FORTRAN

core.o: core.h wrapper.h pnmpimod.h debug.h
debug.o: debug.h
wrapper.o: core.h ../wrapper/wrapper.c ../wrapper/wrapper.h debug.h ../wrapper/f77symbols.h ../wrapper/wrapper_f77.c
wrapperf.o: core.h ../wrapper/wrapper.c ../wrapper/wrapper.h debug.h ../wrapper/f77symbols.h ../wrapper/wrapper_f77.c
	$(MPICC) -c $(CFLAGS) $(INCS) wrapper.c -o wrapperf.o
debug.o: core.h debug.h
services.c: pnmpimod.h core.h debug.h

../wrapper/wrapper.h: ../wrapper/wrapper_h.w
	cd ../wrapper ; $(MAKE) ; cd -

../wrapper/wrapper.c: ../wrapper/wrapper_c.w vargcall.c
	cd ../wrapper ; $(MAKE) ; cd -

.c.o: $(DEPS)
	$(MPICC) -c $(FORTRANFLAG) $(CFLAGS) $(INCS) $<

ifeq ($(findstring AIX,$(OS)),AIX)
libpnmpi.so: $(OBJS)
	$(MPIXX) -o $@ $(SFLAGS) $(OBJS)

forcempi.o: forcempi.c
	$(CC) -c $<

forcempi.a: forcempi.o
	$(AR) $(ARFLAGS) $@ $<
else
ifdef EXPERIMENTAL_UNWIND
libpnmpi.so: $(OBJS)
	$(MPIXX) $(OBJS) -o $@ $(SFLAGS) -lunwind -ldl
else
libpnmpi.so: $(OBJS)
	$(MPIXX) $(OBJS) -o $@ $(SFLAGS) -ldl
endif
endif

ifeq ($(findstring AIX,$(OS)),AIX)
libpnmpif.so: $(FOBJS)
	$(MPIF77) -o $@ $(SFLAGS) $(FOBJS)

forcempi.o: forcempi.c
	$(CC) -c $<

forcempi.a: forcempi.o
	$(AR) $(ARFLAGS) $@ $<
else
ifdef EXPERIMENTAL_UNWIND
libpnmpif.so: $(FOBJS)
	$(MPIF77) $(FOBJS) -o $@ $(SFLAGS) -lunwind -ldl
else
libpnmpif.so: $(FOBJS)
	$(MPIF77) $(FOBJS) -o $@ $(SFLAGS) -ldl
endif
endif

libpnmpi.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpnmpif.a: $(FOBJS)
	$(AR) $(ARFLAGS) $@ $(FOBJS)

install: $(TARGET)
	cp $(TARGET) $(PNMPI_LIB_PATH)
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(TARGET) *.o

clobber: clean
	rm -f *~

