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

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, data;
    MPI_Status status[2];
    MPI_Request sreq[2];
    MPI_Request rreq[2];
    int flag= 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 1) {
/*         MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &sreq[0]); */
/*         MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &sreq[1]); */
/*         while (!flag) */
/*             MPI_Testall(2, sreq, &flag, status); */
        MPI_Send(&data, 1, MPI_INT, 2, 123456, MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_INT, 2, 456789, MPI_COMM_WORLD);

    } else if (rank == 2) {
        MPI_Irecv(&data, 1, MPI_INT, 1, 123456, MPI_COMM_WORLD, &rreq[0]);
        MPI_Irecv(&data, 1, MPI_INT, 1, 456789, MPI_COMM_WORLD, &rreq[1]);
        fprintf(stderr, "rreq[0] = %d\n", rreq[0]);
        fprintf(stderr, "rreq[0] = %d\n", rreq[1]);
        while (!flag)   {
               MPI_Testall(2, rreq, &flag, status);   
        }
        //MPI_Waitall(2, rreq, status);
        //MPI_Wait(&rreq[0], &status[0]);
    }
    MPI_Finalize();
    printf("(%d) Finished normally\n", rank);
    return 0;
}
