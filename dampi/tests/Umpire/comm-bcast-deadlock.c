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
/* Creator: Bronis R. de Supinski (bronis@llnl.gov) Tue Aug 12 2003 */

#ifndef lint
static char *rcsid =
  "$Header: /usr/gapps/asde/cvs-vault/umpire/tests/comm-bcast-deadlock.c,v 1.1 2003/09/02 15:57:49 bronis Exp $";
#endif

#include <stdio.h>
#include <string.h>
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
  MPI_Comm inverted_comm;
  int bcast_rank;

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (comm, &nprocs);
  MPI_Comm_rank (comm, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);

  MPI_Barrier (MPI_COMM_WORLD);

  if (nprocs != 3)
    {
      printf ("Incorrect number of tasks; exactly 3 required\n");
    }
  else {
    /* create inverted communicator... */
    MPI_Comm_split (comm, 0, nprocs - rank, &inverted_comm);

    if (rank == 1) {
      MPI_Bcast (&rank, 1, MPI_INT, 1, inverted_comm);
      MPI_Bcast (&bcast_rank, 1, MPI_INT, 2, comm);
    }
    else if (rank == 2) {
      MPI_Bcast (&rank, 1, MPI_INT, 2, comm);
      MPI_Bcast (&bcast_rank, 1, MPI_INT, 1, inverted_comm);
    }
    else {
      MPI_Bcast (&bcast_rank, 1, MPI_INT, 2, comm);
      MPI_Bcast (&bcast_rank, 1, MPI_INT, 1, inverted_comm);
    }
  }

  MPI_Barrier (comm);

  printf ("(%d) Finished normally\n", rank);

  MPI_Finalize ();
}

/* EOF */
