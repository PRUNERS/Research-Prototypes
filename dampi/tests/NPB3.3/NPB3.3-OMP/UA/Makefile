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
BENCHMARK=ua
BENCHMARKU=UA

include ../config/make.def


OBJS = ua.o convect.o diffuse.o adapt.o move.o mason.o \
       precond.o utils.o verify.o setup.o \
       ${COMMON}/print_results.o ${COMMON}/timers.o ${COMMON}/wtime.o

include ../sys/make.common

# npbparams.h is included by header.h
# The following rule should do the trick but many make programs (not gmake)
# will do the wrong thing and rebuild the world every time (because the
# mod time on header.h is not changed. One solution would be to 
# touch header.h but this might cause confusion if someone has
# accidentally deleted it. Instead, make the dependency on npbparams.h
# explicit in all the lines below (even though dependence is indirect). 

# header.h: npbparams.h

${PROGRAM}: config ${OBJS}
	@if [ x$(UPDATE) = xau ] ; then			\
		${MAKE} PROGRAM=${PROGRAM} ua-au;	\
	else						\
		${MAKE} PROGRAM=${PROGRAM} ua-lk;	\
	fi

ua-lk: ${OBJS} transfer.o
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM} ${OBJS} transfer.o ${F_LIB}

ua-au: ${OBJS} transfer_au.o
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM}.au ${OBJS} transfer_au.o ${F_LIB}

.f.o:
	${FCOMPILE} $<

ua.o:        ua.f       header.h npbparams.h
setup.o:     setup.f    header.h npbparams.h
convect.o:   convect.f  header.h npbparams.h
adapt.o:     adapt.f    header.h npbparams.h
move.o:      move.f     header.h npbparams.h
diffuse.o:   diffuse.f  header.h npbparams.h
mason.o:     mason.f    header.h npbparams.h
precond.o:   precond.f  header.h npbparams.h
transfer.o:  transfer.f header.h npbparams.h
transfer_au.o:  transfer_au.f header.h npbparams.h
utils.o:     utils.f    header.h npbparams.h
verify.o:    verify.f   header.h npbparams.h

clean:
	- rm -f *.o *~ mputil*
	- rm -f npbparams.h core
