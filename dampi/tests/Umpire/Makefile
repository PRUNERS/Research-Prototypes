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

LAUNCHER = ../launcher
#CC = $(LAUNCHER) ispcc
CC = mpicc
ISP = $(LAUNCHER) isp
CFLAGS = -c -g -O2
LDFLAGS = -g -L$(PNMPI_LIB_PATH) -lpnmpi
SOURCES := $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
EXECUTABLES = $(SOURCES:.c=.exe)

all: $(EXECUTABLES)

%.exe: %.o
	$(CC) $< $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLES) $(OBJS)

tests-commit: $(EXECUTABLES)
	@PORT=8000 ; \
	for file in $(EXECUTABLES) ; do \
	    echo "Running $$file... \c" ; \
	    rm -f "$$file.log" ; \
	    $(ISP) -n 3 -p $$PORT -l "$$file.log" ./$$file 1>/dev/null 2>/dev/null ; \
	    if [ $$? -eq 14 ]; then \
	        PORT=`expr $$PORT + 1` ; \
	        rm -f "$$file.log" ; \
	        $(ISP) -n 3 -p $$PORT -l "$$file.log" ./$$file 1>/dev/null 2>/dev/null ; \
	    fi ; \
	    if [ $$? -gt 1 ]; then echo "Failed."; rm -f $$file.log; else echo ""; fi ; \
	    PORT=`expr $$PORT + 1` ; \
	done

tests: $(EXECUTABLES)
	@PORT=8000 ; \
	for file in $(EXECUTABLES) ; do \
	    echo "Testing $$file... \c" ; \
	    rm -f "$$file.log2" ; \
	    $(ISP) -n 3 -p $$PORT -l "$$file.log2" ./$$file 1>/dev/null 2>/dev/null ; \
	    if [ $$? -eq 14 ]; then \
	        PORT=`expr $$PORT + 1` ; \
	        rm -f "$$file.log2" ; \
	        $(ISP) -n 3 -p $$PORT -l "$$file.log2" ./$$file 1>/dev/null 2>/dev/null ; \
	    fi ; \
	    diff -I "[0-9]\+\s\+[0-9]\+\s\+Leak\s\+" $$file.log $$file.log2 >/dev/null ; \
	    if [ $$? -eq 0 ]; then echo "Ok."; else echo "FAILED."; fi ; \
	    PORT=`expr $$PORT + 1` ; \
	done
