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

/* -*- Mode: C; -*- */
/* Creator: Bronis R. de Supinski (bronis@llnl.gov) Fri Mar  17 2000 */
/* no-error-waitany.c -- do some MPI calls without any errors */

#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <string.h>

#define buf_size 128

int
main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  char processor_name[128];
  int namelen = 128;
  int buf0[buf_size];
  int buf1[buf_size];
  int buf2[buf_size];
  int i, flipbit, done;
  MPI_Status status;

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);

  MPI_Barrier (MPI_COMM_WORLD);

  if (nprocs < 2)
    {
      printf ("not enough tasks\n");
    }
  else if (rank == 0)
    {
      MPI_Request reqs[3];

      MPI_Irecv (buf0, buf_size, MPI_INT, 
		 MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &reqs[0]);
      MPI_Irecv (buf1, buf_size, MPI_INT, 
		 MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &reqs[1]);
      MPI_Irecv (buf2, buf_size, MPI_INT, 
		 MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &reqs[2]);

      for (i = 3; i > 0; i--) {
	MPI_Send (&flipbit, 1, MPI_INT, 1, i, MPI_COMM_WORLD);

	MPI_Waitany (i, reqs, &done, &status);

	assert (done == (i - 1));
      }
    }
  else if (rank == 1)
    {
      memset (buf0, 1, buf_size);

      for (i = 3; i > 0; i--) {
	MPI_Recv (&flipbit, 1, MPI_INT, 
		  MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
	
	MPI_Send (buf0, buf_size, MPI_INT, 0, i, MPI_COMM_WORLD);
      }
    }

  MPI_Barrier (MPI_COMM_WORLD);

  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

/* EOF */
