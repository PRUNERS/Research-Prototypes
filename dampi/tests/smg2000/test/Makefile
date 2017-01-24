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
.SUFFIXES: .c .o

srcdir = .

include ../Makefile.include

CFLAGS = \
 -I..\
 -I../utilities\
 -I../struct_mv\
 -I../struct_ls\
 -I../krylov\
 ${INCLUDE_CFLAGS}\
 -DHYPRE_TIMING
 
LFLAGS =\
 -L.\
 -L../struct_ls\
 -L../struct_mv\
 -L../krylov\
 -L../utilities\
 -lHYPRE_struct_ls\
 -lHYPRE_struct_mv\
 -lkrylov\
 -lHYPRE_utilities\
 ${INCLUDE_LFLAGS}

LPFLAGS =\
 -L.\
 -L../struct_ls\
 -L../struct_mv\
 -L../krylov\
 -L../utilities\
 -L${PNMPI_LIB_PATH}\
 -lpnmpi\
 -lHYPRE_struct_ls\
 -lHYPRE_struct_mv\
 -lkrylov\
 -lHYPRE_utilities\
 ${INCLUDE_LFLAGS}

##################################################################
# Targets
##################################################################

all: smg2000 smg2000.pnmpi

clean:
	@rm -f *.o

veryclean: clean
	@rm -f smg2000

##################################################################
# Rules
##################################################################

smg2000: smg2000.o
	@echo  "Linking" $@ "... "
	${CC} -o smg2000 smg2000.o ${LFLAGS}

smg2000.pnmpi: smg2000.o
	@echo  "Linking" $@ "... "
	${CC} -o smg2000.pnmpi smg2000.o ${LPFLAGS}

##################################################################
# Generic rules
##################################################################

.c.o:
	${CC} -o $@ -c ${CFLAGS} $<


