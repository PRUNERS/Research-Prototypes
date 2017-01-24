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
BENCHMARK=sp
BENCHMARKU=SP

include ../config/make.def


OBJS = sp.o make_set.o initialize.o exact_solution.o exact_rhs.o \
       set_constants.o adi.o define.o copy_faces.o rhs.o      \
       lhsx.o lhsy.o lhsz.o x_solve.o ninvr.o y_solve.o pinvr.o    \
       z_solve.o tzetar.o add.o txinvr.o error.o verify.o setup_mpi.o \
       ${COMMON}/print_results.o ${COMMON}/timers.o

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
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM} ${OBJS} ${FMPI_LIB}

.f.o:
	${FCOMPILE} $<

sp.o:             sp.f  header.h npbparams.h  mpinpb.h
make_set.o:       make_set.f  header.h npbparams.h  mpinpb.h
initialize.o:     initialize.f  header.h npbparams.h
exact_solution.o: exact_solution.f  header.h npbparams.h
exact_rhs.o:      exact_rhs.f  header.h npbparams.h
set_constants.o:  set_constants.f  header.h npbparams.h
adi.o:            adi.f  header.h npbparams.h
define.o:         define.f  header.h npbparams.h
copy_faces.o:     copy_faces.f  header.h npbparams.h  mpinpb.h
rhs.o:            rhs.f  header.h npbparams.h
lhsx.o:           lhsx.f  header.h npbparams.h
lhsy.o:           lhsy.f  header.h npbparams.h
lhsz.o:           lhsz.f  header.h npbparams.h
x_solve.o:        x_solve.f  header.h npbparams.h  mpinpb.h
ninvr.o:          ninvr.f  header.h npbparams.h
y_solve.o:        y_solve.f  header.h npbparams.h  mpinpb.h
pinvr.o:          pinvr.f  header.h npbparams.h
z_solve.o:        z_solve.f  header.h npbparams.h  mpinpb.h
tzetar.o:         tzetar.f  header.h npbparams.h
add.o:            add.f  header.h npbparams.h
txinvr.o:         txinvr.f  header.h npbparams.h
error.o:          error.f  header.h npbparams.h  mpinpb.h
verify.o:         verify.f  header.h npbparams.h  mpinpb.h
setup_mpi.o:      setup_mpi.f mpinpb.h npbparams.h 


clean:
	- rm -f *.o *~ mputil*
	- rm -f npbparams.h core
