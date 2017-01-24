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

include ../libconfig.common

CC = mpicc

OBJS = datacorruption.o performance.o communication.o loadconfig.o randapi.o getfaultyprocess.o getrandomnumber.o

CFLAGS = -Wall -O2 -fPIC -g -c -I./include

##################################
# Compile fault injection modules
##################################

all: $(OBJS)

datacorruption.o: datacorruption.c
	$(CC) $(CFLAGS) $?

performance.o: performance.c
	$(CC) $(CFLAGS) $?

communication.o: communication.c
	$(CC) $(CFLAGS) $?

loadconfig.o: loadconfig.c
	$(CC) $(CFLAGS) -I$(LIBCONFIG_H) $?

randapi.o: randapi.c
	$(CC) $(CFLAGS) $?

getfaultyprocess.o: getfaultyprocess.c
	$(CC) -Wall -O2 -g -o getfaultyprocess -I$(LIBCONFIG_H) $? $(LIBCONFIG_FILES)

getrandomnumber.o: getrandomnumber.c
	$(CC) -Wall -O2 -g -I./include -o getrandomnumber getrandomnumber.c randapi.o

clean:
	rm -f *.o getfaultyprocess getrandomnumber
