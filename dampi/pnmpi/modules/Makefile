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

MODS = d_sample d_empty d_datatype d_status d_requests d_comm d_switch-matrix d_virtual d_timelapse

all: $(MODS)

d_sample:
	cd sample ; $(MAKE)  ; cd -

d_empty:
	cd empty ; $(MAKE)  ; cd -

d_switch-matrix:
	cd switch-matrix; $(MAKE)  ; cd -

d_virtual:
	cd virtual; $(MAKE) ; cd -

d_comm:
	cd comm ; $(MAKE)  ; cd -

d_requests:
	cd requests ; $(MAKE)  ; cd -

d_datatype:
	cd datatype ; $(MAKE)  ; cd -

d_status:
	cd status ; $(MAKE)  ; cd -

d_timelapse:
	cd timelapse ; $(MAKE)  ; cd -

d_piggyback:
	cd piggyback ; $(MAKE) ; cd -
clean:
	cd sample ; $(MAKE) clean ; cd -
	cd empty ; $(MAKE) clean ; cd -
	cd datatype ; $(MAKE) clean ; cd -
	cd status ; $(MAKE) clean ; cd -
	cd requests ; $(MAKE) clean ; cd -
	cd comm ; $(MAKE) clean ; cd -
	cd switch-matrix ; $(MAKE) clean ; cd -
	cd timelapse ; $(MAKE) clean ; cd -
	cd virtual ; $(MAKE) clean ; cd -

clobber:
	cd sample ; $(MAKE) clobber ; cd -
	cd empty ; $(MAKE) clobber ; cd -
	cd comm ; $(MAKE) clobber ; cd -
	cd datatype ; $(MAKE) clobber ; cd -
	cd requests ; $(MAKE) clobber ; cd -
	cd status ; $(MAKE) clobber ; cd -
	cd switch-matrix ; $(MAKE) clobber ; cd -
	cd virtual ; $(MAKE) clobber ; cd -


