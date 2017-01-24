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
/* Creator: Bronis R. de Supinski (bronis@llnl.gov) Wed Nov 29 2000 */
/* dropped-req.c -- create a request that's never matched */
/* NOTE: THIS TEST ASSUMES THAT MPI LIBRARY USES EAGER SENDS IF */
/* BUFFER IS ZERO BYTES; WILL DEADLOCK IN WHILE LOOP IF FALSE */


/* NOTE: Some value of ITERATIONS will imply resource exhaustion */
/*       either in Umpire or MPI no matter how things are implemented */
/*       the best we can hope for is to fail gracefully... */
/* 10000 breaks umpire due to running out of memory as of 12/20/02... */
/* FAILURE IS NOT GRACEFUL AS OF THIS TIME... */
#define ITERATIONS                10


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

int
main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  int tag = 31;
  int i;
  MPI_Comm comm = MPI_COMM_WORLD;
  char processor_name[128];
  int namelen = 128;
  MPI_Request req;
  MPI_Status status;

  /* init */
  MPI_Init (&argc, &argv);
  MPI_Comm_size (comm, &nprocs);
  MPI_Comm_rank (comm, &rank);
  MPI_Get_processor_name (processor_name, &namelen);
  printf ("(%d) is alive on %s\n", rank, processor_name);
  fflush (stdout);

  MPI_Barrier(comm);

  /* 0 sends task nprocs-1 a message that is never received */
  if (rank == 0) {
    for (i = 0; i < ITERATIONS; i++) {
      int flag = 0;
      MPI_Isend (&tag, 0, MPI_BYTE, nprocs - 1, tag, comm, &req);

      while (!flag)
	MPI_Test (&req, &flag, &status);
    }
  }

 MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}

/* EOF */
