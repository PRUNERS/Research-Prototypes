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
/* Creator: Bronis R. de Supinski (bronis@llnl.gov)  */
/* no-error-interleaved-isend.c - use isends with vector type */

#ifndef lint
static char *rcsid =
  "$Header: /usr/gapps/asde/cvs-vault/umpire/tests/no-error-interleaved-isend.c,v 1.2 2002/06/07 20:41:22 bronis Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

#define buf_size 128

int
main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  MPI_Comm comm = MPI_COMM_WORLD;
  char processor_name[128];
  int namelen = 128;
  int buf[buf_size];
  int i;
  MPI_Request req;
  MPI_Status status;
  MPI_Datatype strided_type;

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (comm, &nprocs);
  MPI_Comm_rank (comm, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);

  MPI_Type_vector (buf_size/2, 1, 2, MPI_INT, &strided_type);
  MPI_Type_commit (&strided_type);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank < 2)
    {
      for (i = rank; i < buf_size; i = i + 2)
	buf[i] = i;

      MPI_Isend (&buf[rank], 1, 
		 strided_type, (rank + 1) % 2, 0, comm, &req);

      MPI_Recv (&buf[(rank + 1) % 2], 1, 
		strided_type, (rank + 1) % 2, 0, comm, &status);

      MPI_Wait (&req, &status);

      for (i = 0; i < buf_size; i++)
	assert (buf[i] == i);
    }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Type_free (&strided_type);

  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

/* EOF */
