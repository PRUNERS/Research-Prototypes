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

#BHEADER***********************************************************************
# (c) 2000   The Regents of the University of California
#
# See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
# notice, contact person, and disclaimer.
#
#EHEADER***********************************************************************

.SUFFIXES:
.SUFFIXES: .c .f .o

srcdir = .

include ../Makefile.include

CFLAGS =\
 -I..\
 ${INCLUDE_CFLAGS}\
 -DHYPRE_TIMING

RANLIB= ranlib

##################################################################
# Targets
##################################################################

all: libHYPRE_utilities.a

clean:
	@rm -f *.o

veryclean: clean
	@rm -f libHYPRE_*.a

##################################################################
# Utilities rules
##################################################################
 
UTILITIES_HEADERS =\
 memory.h\
 mpistubs.h\
 threading.h\
 timing.h\
 utilities.h\
 HYPRE_utilities.h

UTILITIES_FILES =\
 memory.c\
 mpistubs.c\
 threading.c\
 timer.c\
 timing.c\
 random.c

UTILITIES_OBJS = ${UTILITIES_FILES:.c=.o}
 
libHYPRE_utilities.a: ${UTILITIES_OBJS}
	@echo  "Building $@ ... "
	ar -ru $@ ${UTILITIES_OBJS}
	${RANLIB} $@

${UTILITIES_OBJS}: ${UTILITIES_HEADERS}

timer.o: timer.c
	${CC} -D_POSIX_SOURCE -o $@ -c ${CFLAGS} $<

##################################################################
# Generic rules
##################################################################

.c.o:
	${CC} -o $@ -c ${CFLAGS} $<

