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
/* Creator: Jeffrey Vetter (j-vetter@llnl.gov) Mon Nov  1 1999 */
/* lost-request.c -- overwrite a request and essentially lose a synch point */

#ifndef lint
static char *rcsid =
  "$Header: /usr/gapps/asde/cvs-vault/umpire/tests/change-send-buffer.c,v 1.3 2002/07/30 21:34:42 bronis Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int
main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  int tag1 = 0;
  int tag2 = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  char processor_name[128];
  int namelen = 128;
  int buf0[128];
  int buf1[128];
  int i;
  MPI_Request aReq[2];
  MPI_Status aStatus[2];

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (comm, &nprocs);
  MPI_Comm_rank (comm, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);
int j, k;
  for (i = 0; i < 128; i++)
    {
      buf0[i] = i;
      buf1[i] = 127 - i;
    }

  MPI_Barrier(MPI_COMM_WORLD);

  switch (rank)
    {
    case 0:
       MPI_Isend (buf0, 128, MPI_INT, 1, tag1, comm, &aReq[0]);
      MPI_Isend (buf1, 128, MPI_INT, 1, tag2, comm, &aReq[1]);
      /* do some work here */

      buf0[64] = 1000000;

      MPI_Wait (&aReq[0], &aStatus[0]);
      MPI_Wait (&aReq[1], &aStatus[1]);

      break;

    case 1:
      MPI_Irecv (buf0, 128, MPI_INT, 0, tag1, comm, &aReq[0]);
      MPI_Irecv (buf1, 128, MPI_INT, 0, tag2, comm, &aReq[1]);
      /* do some work here ... */
      MPI_Wait (&aReq[0], &aStatus[0]);
      MPI_Wait (&aReq[1], &aStatus[1]);
      break;

    default:
      /* do nothing */
      break;
    }


  MPI_Barrier(MPI_COMM_WORLD);

  for (i = 0; i < 128; i++)
    {
      buf0[i] = i;
      buf1[i] = 127 - i;
    }
  switch (rank)
    {
    case 0:
      MPI_Isend (buf0, 128, MPI_INT, 1, tag1, comm, &aReq[0]);
      MPI_Isend (buf1, 128, MPI_INT, 1, tag2, comm, &aReq[1]);
      /* do some work here */

      buf0[64] = 1000000;

      MPI_Waitall (2, aReq, aStatus);

      break;

    case 1:
      MPI_Irecv (buf0, 128, MPI_INT, 0, tag1, comm, &aReq[0]);
      MPI_Irecv (buf1, 128, MPI_INT, 0, tag2, comm, &aReq[1]);
      /* do some work here ... */
      MPI_Waitall (2, aReq, aStatus);
      break;

    default:
      /* do nothing */
      break;
    }

  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

/* EOF */
