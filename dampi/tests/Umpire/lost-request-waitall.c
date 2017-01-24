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
  "$Header: /usr/gapps/asde/cvs-vault/umpire/tests/lost-request-waitall.c,v 1.1.1.1 2000/08/23 17:28:26 vetter Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#define buf_size 128

mydelay ()			/* about 6 seconds */
{
  int i;
  int val;
  for (i = 0; i < 3000000; i++)
    {
      val = getpid ();		/* about 2.06 usecs on snow */
    }
  return val;
}

int
main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  int tag1 = 31;
  int tag2 = 32;
  MPI_Comm comm = MPI_COMM_WORLD;
  char processor_name[128];
  int namelen = 128;
  int buf0[buf_size];
  int buf1[buf_size];
  MPI_Request req, req0, req1;
  MPI_Status status, status0, status1;
  MPI_Request areq[10];
  MPI_Status astatus[10];

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (comm, &nprocs);
  MPI_Comm_rank (comm, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);

  memset (buf0, 0, buf_size);
  memset (buf1, 1, buf_size);

  /* 0 sends 1 two messages, but the request gets overwritten */
  switch (rank)
    {
    case 0:
      MPI_Isend (buf0, buf_size, MPI_INT, 1, tag1, comm, &(areq[0]));
      MPI_Isend (buf1, buf_size, MPI_INT, 1, tag2, comm, &(areq[1]));
      /* do some work here */
      //mydelay ();
      MPI_Waitall (2, areq, astatus);
      break;

    case 1:
      MPI_Irecv (buf0, buf_size, MPI_INT, 0, tag1, comm, &req);
      MPI_Irecv (buf1, buf_size, MPI_INT, 0, tag2, comm, &req);	/* overwrite req */
      /* do some work here and get confused */
      MPI_Wait (&req, &status);
      break;

    default:
      /* do nothing */
      break;
    }

  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

/* EOF */
