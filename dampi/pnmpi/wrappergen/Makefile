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

# Generated automatically from Makefile.in by configure.
#  (C) 1993 by Argonne National Laboratory and Mississipi State University.
#

include $(PROJECTS)/common/Makefile.common

EXECS = wrappergen write_proto
MPI_PROTO = '-DPROTO_FILE="../wrapper/mpi_proto_def"'

default: wrappergen

all: $(EXECS)

readproto.o: readproto.c wrappergen.h
	$(CC) $(CFLAGS) $(MPI_PROTO) -c $*.c

wrappergen.o: wrappergen.c wrappergen.h
	$(CC) $(CFLAGS) -c $*.c

wrappergen: readproto.o wrappergen.o args.o
	$(LD) -o wrappergen $(CFLAGS) readproto.o \
	   wrappergen.o args.o

clean:
	-rm -rf $(EXECS) *.o

clobber: clean
	-rm *~
