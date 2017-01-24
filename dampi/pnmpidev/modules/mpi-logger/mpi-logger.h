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

#include <mpi.h>

#ifndef _PNMPI_MOD_MPI_LOGGER
#define _PNMPI_MOD_MPI_LOGGER

/*------------------------------------------------------------*/
/* Note: this module must be loaded into the stack before
   any module using it (i.e., must be instantiated between
   any user and the application). Also, any call to its
   services must occur after calling PMPI_Init and services
   are only available to modules requesting it during
   MPI_Init */

/*------------------------------------------------------------*/
#define PNMPI_MODULE_MPI_LOGGER "mpi-logger"


/*..........................................................*/
/* Access macros */

enum {SEND,BSEND,RSEND,SSEND,ISEND,IBSEND,IRSEND,ISSEND,RECV,PROBE,IPROBE,IRECV,SENDRECV,SENDRECV_REPLACE,WAIT,WAITANY,WAITSOME,WAITALL,TEST,TESTANY,TESTSOME,TESTALL,BCAST,SCATTER,SCATTERV,GATHER,GATHERV,REDUCE,ALLGATHER,ALLGATHERV,ALLTOALL,ALLTOALLV,ALLREDUCE,REDUCE_SCATTER,BARRIER,SCAN,COMM_DUP,COMM_FREE,COMM_CREATE,COMM_SPLIT,CART_CREATE,CART_SUB};

enum {IPROBE_ANY,PROBE_ANY,RECV_ANY,IRECV_ANY};

enum {size256,size512,size1K,size2K,size4K,size8K,size16K,size32K,size64K,size128K,above};
#endif /* _PNMPI_MOD_COMM_MAPPING */
