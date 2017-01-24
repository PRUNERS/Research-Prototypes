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

TARGET = prep d_patch d_wrappergen d_wrapper d_src d_modules d_demo

all: $(TARGET)

prep:
	mkdir -p lib
	mkdir -p $(PNMPI_LIB_PATH)
	mkdir -p $(PNMPI_INC_PATH)

d_patch:
	cd patch; $(MAKE)

d_wrappergen:
	cd wrappergen; $(MAKE)

d_wrapper:
	cd wrapper; $(MAKE)

d_src:
	cd src; $(MAKE)

d_modules:
	cd modules; $(MAKE)

d_demo:
	cd demo; $(MAKE)

clean:
	cd patch; $(MAKE) clean
	cd wrappergen; $(MAKE) clean
	cd wrapper; $(MAKE) clean
	cd src; $(MAKE) clean
	cd modules; $(MAKE) clean
	cd demo; $(MAKE) clean
	rm -rf lib
	rm -rf include/deadlock-check.h
	rm -rf include/mpi-logger.h

clobber:
	cd patch; $(MAKE) clobber
	cd wrappergen; $(MAKE) clobber
	cd wrapper; $(MAKE) clobber
	cd src; $(MAKE) clobber
	cd modules; $(MAKE) clobber
	cd demo; $(MAKE) clobber
	rm -f *~
