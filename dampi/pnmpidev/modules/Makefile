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

all: d_comm d_comm-mapping d_dampi d_dampi_vc d_deadlock-check d_maxtime d_mpi-logger d_onoff d_piggyback
# d_injector d_papi

d_comm:
	cd comm; make all

d_comm-mapping:
	cd comm-mapping; make all

d_dampi:
	cd dampi; make all

d_dampi_vc:
	cd dampi_vc; make all

d_deadlock-check:
	cd deadlock-check; make all

#d_injector:
#	cd injector; make all

d_maxtime:
	cd maxtime; make all

d_mpi-logger:
	cd mpi-logger; make all

d_onoff:
	cd onoff; make all

# d_papi:
# 	cd papi; make all

d_piggyback:
	cd piggyback; make all

install:
	cd comm; make install
	cd comm-mapping; make install
	cd dampi; make install
	cd dampi_vc; make install
	cd deadlock-check; make install
	# cd injector; make install
	cd maxtime; make install
	cd mpi-logger; make install
	cd onoff; make install
	# cd papi; make install
	cd piggyback; make install

clean:
	cd comm; make clean
	cd comm-mapping; make clean
	cd dampi; make clean
	cd dampi_vc; make clean
	cd deadlock-check; make clean
	# cd injector; make clean
	cd maxtime; make clean
	cd mpi-logger; make clean
	cd onoff; make clean
	# cd papi; make clean
	cd piggyback; make clean
