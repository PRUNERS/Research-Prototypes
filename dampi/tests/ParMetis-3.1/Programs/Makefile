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

include ../Makefile.in

BINDIR = ../Graphs

INCLUDES = -I./ -I../ParMETISLib $(INCDIR) 
CFLAGS = -DSEND_BLOCK $(COPTIONS) $(OPTFLAGS) $(INCLUDES)

LIBSDIR = -L.. $(LIBDIR) 
LIBS = -lparmetis -lmetis $(XTRALIBS) -lm -L$(PNMPI_LIB_PATH) -lpnmpi
PARMETISLIB = ../libparmetis.a ../libmetis.a


PTESTOBJS = ptest.o io.o adaptgraph.o
MESHTESTOBJS = mtest.o io.o
PARMETISOBJS = parmetis.o io.o adaptgraph.o

          
.c.o:
	$(CC) $(CFLAGS) -c $*.c


default: $(BINDIR)/ptest$(VERNUM) $(BINDIR)/mtest$(VERNUM) 

$(BINDIR)/mtest$(VERNUM): $(MESHTESTOBJS) $(ISPOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(MESHTESTOBJS) $(ISPOBJS2) $(LIBSDIR) $(LIBS)
	chmod 744 $@

$(BINDIR)/ptest$(VERNUM): $(PTESTOBJS) $(ISPOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(PTESTOBJS) $(ISPOBJS2) $(LIBSDIR) $(LIBS)
	chmod 744 $@

clean:
	rm -f *.o ;\
	rm -f $(BINDIR)/ptest$(VERNUM) 
	rm -f $(BINDIR)/mtest$(VERNUM) 

realclean:
	rm -f *.o ;\
	rm -f $(BINDIR)/ptest$(VERNUM) 
	rm -f $(BINDIR)/mtest$(VERNUM) 


checkin:
	@for file in *.[c,h]; \
	do \
	ci -u -m'Maintance' $$file;\
	done 

checkin2:
	@for file in *.[c,h]; \
	do \
	ci $$file;\
	rcs -U $$file;\
	co $$file;\
	done
