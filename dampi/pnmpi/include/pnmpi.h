/*
Copyright (c) 2015
Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.
Written by Anh Vo, Sriram Aananthakrishnan, Ganesh Gopalakrishnan,
Bronis R. de Supinski, Martin Schulz, and Greg Bronevetsky
Contact email: schulzm@llnl.gov, LLNL-CODE-647203
All rights reserved - please read information in "LICENSE".

This file is part of DAMPI. For details see
https://github.com/soarlab/DAMPI.

Please also read the file "LICENSE" included in this package for Our
Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License (as published by
the Free Software Foundation) version 2.1 dated February 1999.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
conditions of the GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the

Free Software Foundation, Inc., 
59 Temple Place, Suite 330, 
Boston, MA 02111-1307 USA
*/

/* Public interface for PNMPI */
/* To be used by applications that are aware of PNMPI */

#include <mpi.h>

#include "pnmpimod.h"

#define PNMPI_PCONTROL_LEVEL 3

#define PNMPI_PCONTROL_SINGLE   0x0
#define PNMPI_PCONTROL_MULTIPLE 0x1
#define PNMPI_PCONTROL_VARG     0x0
#define PNMPI_PCONTROL_PTR      0x2

#define PNMPI_PCONTROL_PMPI   0
#define PNMPI_PCONTROL_OFF    1
#define PNMPI_PCONTROL_ON     2
#define PNMPI_PCONTROL_PNMPI  3
#define PNMPI_PCONTROL_MIXED  4
#define PNMPI_PCONTROL_INT    5

#define PNMPI_ERROR -1
