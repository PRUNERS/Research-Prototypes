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

/* $Header: /usr/gapps/asde/cvs-vault/umpire/tests/deadlock-config.c,v 1.2 2001/09/20 22:27:28 bronis Exp $ */

#include <stdio.h>
#include <mpi.h>

#define buf_size 32000

main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  float data[buf_size];
  int tag = 30;
  char processor_name[128];
  int namelen = buf_size;

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  if (rank == 0)
    {
      printf ("WARNING: This test depends on the MPI's eager limit. "
	      "Set it appropriately.\n");
    }
  printf ("Initializing (%d of %d)\n", rank, nprocs);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);
  {
    int dest = (rank == nprocs - 1) ? (0) : (rank + 1);
    data[0] = rank;
    MPI_Send (data, buf_size, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
    printf ("(%d) sent data %f\n", rank, data[0]);
    fflush (stdout);
  }
  {
    int src = (rank == 0) ? (nprocs - 1) : (rank - 1);
    MPI_Status status;
    MPI_Recv (data, buf_size, MPI_FLOAT, src, tag, MPI_COMM_WORLD, &status);
    printf ("(%d) got data %f\n", rank, data[0]);
    fflush (stdout);
  }
  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

				 /* EOF */
