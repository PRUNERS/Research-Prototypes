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

/* compute pi using Monte Carlo method */ 
#include <math.h> 
#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>
#define CHUNKSIZE      1000 
/* We'd like a value that gives the maximum value returned by the function 
   random, but no such value is *portable*.  RAND_MAX is available on many  
   systems but is not always the correct value for random (it isn't for  
   Solaris).  The value ((unsigned(1)<<31)-1) is common but not guaranteed */ 
#define INT_MAX 1000000000 
 
/* message tags */ 
#define REQUEST  1 
#define REPLY    2 
int total = 0;
int main( int argc, char *argv[] ) 
{ 
  int iter; 
  int in, out, i, max, ranks[1], done; 
  double x, y, Pi, error, epsilon; 
  int numprocs, myid, server, totalin, totalout, workerid; 
  int rands[CHUNKSIZE], request; 
  MPI_Comm world, workers; 
  MPI_Group world_group, worker_group; 
  MPI_Status status; 
 
  MPI_Init(&argc,&argv); 
  world  = MPI_COMM_WORLD; 
  MPI_Comm_size(world,&numprocs); 
  MPI_Comm_rank(world,&myid); 
  server = numprocs-1;	/* last proc is server */ 
  if (myid == 0) 
    epsilon = 0.5; //sscanf( argv[1], "%lf", &epsilon ); 
  MPI_Bcast( &epsilon, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD ); 
  MPI_Comm_group( world, &world_group ); 
  ranks[0] = server; 
  MPI_Group_excl( world_group, 1, ranks, &worker_group ); 
  MPI_Comm_create( world, worker_group, &workers ); 
  MPI_Group_free(&worker_group);
  if (myid == server) {	/* I am the rand server */ 
    //        MPI_Pcontrol(1);
    do { 
	    MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST, 
               world, &status); 
      if (status.MPI_SOURCE==0)
        total++;
	    if (request) { 
        for (i = 0; i < CHUNKSIZE; ) { 
          rands[i] = random(); 
          if (rands[i] <= INT_MAX) i++; 
        } 
        MPI_Send(rands, CHUNKSIZE, MPI_INT, 
                 status.MPI_SOURCE, REPLY, world); 
	    } 
    } 
    while( request>0 ); 
    //        MPI_Pcontrol(1);
  } 
  else {			/* I am a worker process */ 
    request = 1; 
    done = in = out = 0; 
    max  = INT_MAX;         /* max int, for normalization */
    MPI_Send( &request, 1, MPI_INT, server, REQUEST, world ); 
    MPI_Comm_rank( workers, &workerid ); 
    iter = 0; 
    while (!done) { 
	    iter++; 
	    request = 1; 
	    MPI_Recv( rands, CHUNKSIZE, MPI_INT, server, REPLY, 
                world, &status ); 
	    for (i=0; i<CHUNKSIZE-1; ) { 
        x = (((double) rands[i++])/max) * 2 - 1; 
        y = (((double) rands[i++])/max) * 2 - 1; 
        if (x*x + y*y < 1.0) 
          in++; 
        else 
          out++; 
	    } 
	    MPI_Allreduce(&in, &totalin, 1, MPI_INT, MPI_SUM, 
                    workers); 
	    MPI_Allreduce(&out, &totalout, 1, MPI_INT, MPI_SUM, 
                    workers); 
	    Pi = (4.0*totalin)/(totalin + totalout); 
	    error = fabs( Pi-3.141592653589793238462643); 
	    done = (error < epsilon || (totalin+totalout) > 1000000); 
	    request = (done) ? 0 : 1; 
	    if (myid == 0) { 
        printf( "\rpi = %23.20f", Pi ); 
        MPI_Send( &request, 1, MPI_INT, server, REQUEST, 
                  world ); 
	    } 
	    else { 
        if (request) 
          MPI_Send(&request, 1, MPI_INT, server, REQUEST, 
                   world); 
	    } 
    } 
    MPI_Comm_free(&workers); 
  } 
 
  if (myid == 0) { 
    printf( "\npoints: %d\nin: %d, out: %d, <ret> to exit\n", 
            totalin+totalout, totalin, totalout ); 
  } else if (myid==server) {
  }
  MPI_Finalize(); 
  return 0;
} 
