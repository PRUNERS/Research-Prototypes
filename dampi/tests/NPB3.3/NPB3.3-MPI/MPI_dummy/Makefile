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

# Makefile for MPI dummy library. 
# Must be edited for a specific machine. Does NOT read in 
# the make.def file of NPB 2.3
F77 = f77
CC = cc
AR = ar

# Enable if either Cray or IBM: (no such flag for most machines: see wtime.h)
# MACHINE	=	-DCRAY
# MACHINE	=	-DIBM

libmpi.a: mpi_dummy.o mpi_dummy_c.o wtime.o
	$(AR) r libmpi.a mpi_dummy.o mpi_dummy_c.o wtime.o

mpi_dummy.o: mpi_dummy.f mpif.h
	$(F77) -c mpi_dummy.f
# For a Cray C90, try:
#	cf77 -dp -c mpi_dummy.f
# For an IBM 590, try:
#	xlf -c mpi_dummy.f

mpi_dummy_c.o: mpi_dummy.c mpi.h
	$(CC) -c ${MACHINE} -o mpi_dummy_c.o mpi_dummy.c

wtime.o: wtime.c
# For most machines or CRAY or IBM
	$(CC) -c ${MACHINE} wtime.c
# For a precise timer on an SGI Power Challenge, try:
#	$(CC) -o wtime.o -c wtime_sgi64.c

test: test.f
	$(F77) -o test -I. test.f -L. -lmpi



clean: 
	- rm -f *~ *.o
	- rm -f test libmpi.a
