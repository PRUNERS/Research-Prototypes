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
BENCHMARK=dc
BENCHMARKU=DC

include ../config/make.def

include ../sys/make.common

OBJS = adc.o dc.o extbuild.o rbt.o jobcntl.o \
	${COMMON}/c_print_results.o  \
	${COMMON}/c_timers.o ${COMMON}/c_wtime.o


# npbparams.h is provided for backward compatibility with NPB compilation
# header.h: npbparams.h

${PROGRAM}: config ${OBJS} 
	${CLINK} ${CLINKFLAGS} -o ${PROGRAM} ${OBJS} ${C_LIB}

.c.o:
	${CCOMPILE} $<

adc.o:      adc.c npbparams.h
dc.o:       dc.c adcc.h adc.h macrodef.h npbparams.h
extbuild.o: extbuild.c adcc.h adc.h macrodef.h npbparams.h
rbt.o:      rbt.c adcc.h adc.h rbt.h macrodef.h npbparams.h
jobcntl.o:  jobcntl.c adcc.h adc.h macrodef.h npbparams.h

clean:
	- rm -f *.o 
	- rm -f npbparams.h core
	- rm -f {../,}ADC.{logf,view,dat,viewsz,groupby,chunks}.* 

