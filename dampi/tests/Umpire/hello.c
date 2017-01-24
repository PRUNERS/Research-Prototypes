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

/* -*- C -*- 

   @PROLOGUE@

   ----- 

   Jeffrey Vetter vetter@llnl.gov
   Center for Applied Scientific Computing, LLNL
   31 Oct 2000

   hello.c -- simple hello world app

 */

#ifndef lint
static char *rcsid = "$Header: /usr/gapps/asde/cvs-vault/umpire/tests/hello.c,v 1.2 2000/12/04 19:09:46 bronis Exp $";
#endif

#include <stdio.h>
#include <mpi.h>

main (int argc, char **argv)
{
  int nprocs = -1;
  int rank = -1;
  int recvbuf = 0;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  printf ("MPI comm size is %d with rank %d executing\n", nprocs, rank);
  MPI_Barrier (MPI_COMM_WORLD);
  MPI_Reduce (&rank, &recvbuf, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    {
      printf ("Reduce max is %d\n", recvbuf);
    }
  MPI_Barrier (MPI_COMM_WORLD);
  MPI_Finalize ();
  printf ("(%d) Finished normally\n", rank);
}


/* eof */
