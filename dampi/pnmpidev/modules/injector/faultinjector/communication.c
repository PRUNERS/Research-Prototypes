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

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "faultinjector.h"

/*
 *-----------------------------------------------
 * Fault injection functions
 *-----------------------------------------------
 */

int 
sendAsynchExtraMessages(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
			MPI_Comm comm, unsigned int num)
{
    unsigned int i;
    MPI_Request *req;
    req = (MPI_Request *)malloc(sizeof(MPI_Request) * num);

    printf("\n[FI] Sending %d Asynch extra messages..\n", num);
    for (i = 0; i < num; i++) {
	PMPI_Isend(buf, count, datatype, dest, tag, comm, ((MPI_Request *)req + i));
    }
    printf("[FI] ..done with the %d Asynch extra messages.\n", num);	

    return 1;
}

int
sendP2PExtraMessages(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
		     MPI_Comm comm, unsigned int num)
{
    unsigned int i;
    //MPI_Request *req;
    //req = (MPI_Request *)malloc(sizeof(MPI_Request) * num);

    printf("\n[FI] Sending %d P2P extra messages..\n", num);
    for (i = 0; i < num; i++) {
	PMPI_Send(buf, count, datatype, dest, tag, comm);
    }
    printf("[FI] ..done with the %d P2P extra messages.\n", num);

    return 1;
}

int 
discardMessages(void)
{
    return 1;
}

/*
 *-----------------------------------------------
 * Utility functions
 *-----------------------------------------------
 */

