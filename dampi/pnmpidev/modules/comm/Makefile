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

PREFIX = commsub

SUBMOD = cptime piggyback

OFILES = $(addsuffix .o, $(addprefix $(PREFIX)-,$(SUBMOD)))
CFILES = $(addsuffix .cxx, $(addprefix $(PREFIX)-,$(SUBMOD)))
MOD    = $(addsuffix .so,$(addprefix $(PREFIX)-,$(SUBMOD)))


CFLAGS += -I$(PNMPI_INC_PATH) -fPIC -I$(PNMPIBASE)/modules/comm
CCFLAGS += -I$(PNMPI_INC_PATH) -fPIC -I$(LOCAL)/include -I$(OPENSS_BASE)/include -I$(PNMPIBASE)/modules/comm


all: $(MOD) install

$(OFILES): $(PNMPIBASE)/modules/comm/comm.o

.SUFFIXES: .c .o .so

.o.so: $(PNMPIBASE)/modules/comm/comm.o
	$(CXX) -o $@ $< $(SFLAGS)

.c.o:
	$(MPICC) -c $(CFLAGS) -DSUBMODNAME=\"$(basename $(subst $(PREFIX)-,,$@))\" $<

.cxx.o:
	$(MPIXX) -c $(CCFLAGS) -DSUBMODNAME=\"$(basename $(subst $(PREFIX)-,,$@))\" $<

$(PNMPIBASE)/modules/comm/comm.o: $(PNMPIBASE)/modules/comm/comm.cxx
	$(MPIXX) -c $(CCFLAGS) $< -o $@

install: $(MOD)
	for mymod in $(MOD); do ($(PNMPIBASE)/patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
#	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

