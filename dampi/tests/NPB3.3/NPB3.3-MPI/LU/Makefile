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

SHELL=/bin/sh
BENCHMARK=lu
BENCHMARKU=LU
VEC=

include ../config/make.def

OBJS = lu.o init_comm.o read_input.o bcast_inputs.o proc_grid.o neighbors.o \
       nodedim.o subdomain.o setcoeff.o sethyper.o setbv.o exact.o setiv.o \
       erhs.o ssor.o exchange_1.o exchange_3.o exchange_4.o exchange_5.o \
       exchange_6.o rhs.o l2norm.o jacld.o blts$(VEC).o jacu.o buts$(VEC).o \
       error.o pintgr.o verify.o ${COMMON}/print_results.o ${COMMON}/timers.o

include ../sys/make.common


# npbparams.h is included by applu.incl
# The following rule should do the trick but many make programs (not gmake)
# will do the wrong thing and rebuild the world every time (because the
# mod time on header.h is not changed. One solution would be to 
# touch header.h but this might cause confusion if someone has
# accidentally deleted it. Instead, make the dependency on npbparams.h
# explicit in all the lines below (even though dependence is indirect). 

# applu.incl: npbparams.h

${PROGRAM}: config
	@if [ x$(VERSION) = xvec ] ; then	\
		${MAKE} VEC=_vec exec;		\
	elif [ x$(VERSION) = xVEC ] ; then	\
		${MAKE} VEC=_vec exec;		\
	else					\
		${MAKE} exec;			\
	fi

exec: $(OBJS)
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM} ${OBJS} ${FMPI_LIB}

.f.o :
	${FCOMPILE} $<

lu.o:		lu.f applu.incl npbparams.h
bcast_inputs.o:	bcast_inputs.f applu.incl npbparams.h mpinpb.h
blts$(VEC).o:	blts$(VEC).f
buts$(VEC).o:	buts$(VEC).f
erhs.o:		erhs.f applu.incl npbparams.h
error.o:	error.f applu.incl npbparams.h mpinpb.h
exact.o:	exact.f applu.incl npbparams.h
exchange_1.o:	exchange_1.f applu.incl npbparams.h mpinpb.h
exchange_3.o:	exchange_3.f applu.incl npbparams.h mpinpb.h
exchange_4.o:	exchange_4.f applu.incl npbparams.h mpinpb.h
exchange_5.o:	exchange_5.f applu.incl npbparams.h mpinpb.h
exchange_6.o:	exchange_6.f applu.incl npbparams.h mpinpb.h
init_comm.o:	init_comm.f applu.incl npbparams.h mpinpb.h 
jacld.o:	jacld.f applu.incl npbparams.h
jacu.o:		jacu.f applu.incl npbparams.h
l2norm.o:	l2norm.f mpinpb.h
neighbors.o:	neighbors.f applu.incl npbparams.h
nodedim.o:	nodedim.f
pintgr.o:	pintgr.f applu.incl npbparams.h mpinpb.h
proc_grid.o:	proc_grid.f applu.incl npbparams.h
read_input.o:	read_input.f applu.incl npbparams.h mpinpb.h
rhs.o:		rhs.f applu.incl npbparams.h
setbv.o:	setbv.f applu.incl npbparams.h
setiv.o:	setiv.f applu.incl npbparams.h
setcoeff.o:	setcoeff.f applu.incl npbparams.h
sethyper.o:	sethyper.f applu.incl npbparams.h
ssor.o:		ssor.f applu.incl npbparams.h mpinpb.h
subdomain.o:	subdomain.f applu.incl npbparams.h mpinpb.h
verify.o:	verify.f applu.incl npbparams.h

clean:
	- /bin/rm -f npbparams.h
	- /bin/rm -f *.o *~
