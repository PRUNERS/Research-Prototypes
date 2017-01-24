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
#include "commsub.h"

#define CSD


/*--------------------------------------------------------------------------*/
/* apply checksum to all messages */

long get_checksum(void *buf, int count, MPI_Datatype dt)
{
  return 0;
}

void COMM_P2P_INIT(int argc, char**argv)
{
}


void SEND_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag, MPI_Comm comm, void **ptr)
{
}

void SEND_P2P_MID(void *buf, int count, MPI_Datatype dt, int node, int tag, MPI_Comm comm, void **ptr)
{
  long checksum;

  if (comm!=MPI_COMM_NULL)
    {
      checksum=get_checksum(buf,count,dt);
      PMPI_Send(&checksum,1,MPI_LONG,node,tag,comm);
    }
}

void SEND_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag, MPI_Comm comm, int err, void **ptr)
{
}

void RECV_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag, MPI_Comm comm, void **ptr)
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag, MPI_Comm comm, int err, void **ptr)
{
  long checksum,incoming;
  MPI_Status status;

  if (comm!=MPI_COMM_NULL)
    {
      checksum=get_checksum(buf,count,dt);
      PMPI_Recv(&incoming,1,MPI_LONG,node,tag,comm,&status);
      
      if (checksum==incoming)
	{
	  if ((abs(verbosity_level) & 0xff)>=2)
	    {
	      printf("Checksum OK\n");
	    }
	}
      else
	{
	  printf("Checksum Error - %li instead of %li (dt=%i)\n",checksum,incoming,dt);
	}

    }
}



