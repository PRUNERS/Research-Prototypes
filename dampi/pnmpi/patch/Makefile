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

TARGET = patch

all: $(TARGET)

ifeq ($(findstring AIX,$(OS)),AIX)
CFLAGS += -I/usr/global/tools/gnu/binutils/aix_5_ll/binutils-2.15-64bit/include/ 
CFLAGS += -D$(OS)
#LFLAGS += -L/usr/global/tools/gnu/binutils/aix_5_ll/binutils-2.15-64bit/lib/
else
CFLAGS += -D$(OS)
endif

CFLAGS += -I/usr/local/binutils-2.20/include
DYNLFLAGS += $(LFLAGS)
ifeq ($(findstring Linux,$(OS)),Linux)
CFLAGS += -DBFD_COPY
DYNLFLAGS += -lbfd -liberty -lz
else
CFLAGS += -DDIRECT_COPY
endif

patch.o: patch.c
	$(CC) -c $(CFLAGS) $< 

patch: patch.o
	$(LD) -o $@ $< $(DYNLFLAGS)

clean:
	rm -f $(TARGET) *.o

clobber: clean
	rm -f *~
