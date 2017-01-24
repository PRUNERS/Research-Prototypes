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
CFLAGS = $(COPTIONS) $(OPTFLAGS) $(INCLUDES)


LIBSDIR = -L.. $(LIBDIR) 
LIBS = -lparmetis -lmetis $(XTRALIBS) -lm
PARMETISLIB = ../libparmetis.a ../libmetis.a


PTESTOBJS = ptest.o io.o adaptgraph.o 
MESHTESTOBJS = mtest.o io.o
PARMETISOBJS = parmetis.o io.o adaptgraph.o
POMETISOBJS = pometis.o io.o 

          
.c.o:
	$(CC) $(CFLAGS) -c $*.c


default: $(BINDIR)/ptest$(VERNUM) $(BINDIR)/mtest$(VERNUM) $(BINDIR)/parmetis$(VERNUM) \
         $(BINDIR)/pometis$(VERNUM) $(BINDIR)/ptest.pnmpi

$(BINDIR)/mtest$(VERNUM): $(MESHTESTOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(MESHTESTOBJS) $(LIBSDIR) $(LIBS)
	chmod 744 $@

$(BINDIR)/ptest$(VERNUM): $(PTESTOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(PTESTOBJS) $(LIBSDIR) $(LIBS)
	chmod 744 $@

$(BINDIR)/ptest.pnmpi: $(PTESTOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(PTESTOBJS) $(LIBSDIR) $(LIBS) -L$(PNMPI_LIB_PATH) -lpnmpi
	chmod 744 $@

$(BINDIR)/parmetis$(VERNUM): $(PARMETISOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(PARMETISOBJS) $(LIBSDIR) $(LIBS)
	chmod 744 $@

$(BINDIR)/pometis$(VERNUM): $(POMETISOBJS) $(PARMETISLIB)
	$(LD)  -o $@ $(POMETISOBJS) $(LIBSDIR) $(LIBS)
	chmod 744 $@


clean:
	rm -f *.o ;\
	rm -f $(BINDIR)/ptest$(VERNUM) 
	rm -f $(BINDIR)/mtest$(VERNUM) 
	rm -f $(BINDIR)/parmetis$(VERNUM) 
	rm -f $(BINDIR)/pometis$(VERNUM) 

realclean:
	rm -f *.o ;\
	rm -f $(BINDIR)/ptest$(VERNUM) 
	rm -f $(BINDIR)/mtest$(VERNUM) 
	rm -f $(BINDIR)/parmetis$(VERNUM) 
	rm -f $(BINDIR)/pometis$(VERNUM) 

