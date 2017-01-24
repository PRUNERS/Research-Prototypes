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

LBITS := $(shell getconf LONG_BIT)
LOGFILE = ptest.log
ifeq ($(LBITS), 64)
	LOGFILE = ptest.log.64bit
endif

default:
	(cd METISLib ; make ) 
	(cd ParMETISLib ; make ) 
	(cd Programs ; make ) 

clean:
	(cd METISLib ; make realclean )
	(cd ParMETISLib ; make realclean )
	(cd Programs ; make realclean )

tests-commit:
	@cd Graphs ; \
	echo "Running ParMetis (Unix sockets enabled)... \c" ; \
	rm -f $(LOGFILE) ; \
	../../launcher isp -n 2 -x -l $(LOGFILE) ./ptest 1>/dev/null 2>/dev/null ; \
	if [ $$? -gt 1 ]; then echo "Failed."; rm -f ptest.log; else echo ""; fi

tests:
	@cd Graphs ; \
	echo "Testing ParMetis (Unix sockets enabled)... \c" ; \
	rm -f "ptest.log2" ; \
	../../launcher isp -n 2 -x -l "ptest.log2" ./ptest 1>/dev/null 2>/dev/null ; \
	diff -I "[0-9]\+\s\+[0-9]\+\s\+Leak\s\+" $(LOGFILE) ptest.log2 >/dev/null ; \
	if [ $$? -eq 0 ]; then echo "Ok."; else echo "FAILED."; fi
