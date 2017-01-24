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

#include "mpi.h"
#include <stdio.h>
#include <string.h>


/* Global variables */
struct {
        char name[10];
        int array[5];
        double f1, f2;
        int val;
} data;

void printData(char *, int);

int main( int argc, char *argv[] )
{
        int myid, numprocs, i;
        MPI_Status status;
	MPI_Request req;

        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
        MPI_Comm_rank(MPI_COMM_WORLD,&myid);


        /* Data structure */
        int blockcounts[4] = {10, 5, 2, 1};
        MPI_Datatype types[4];
        MPI_Aint displs[4];
        MPI_Datatype newtype;

        MPI_Address( &data.name, &displs[0] );
        MPI_Address( &data.array, &displs[1] );
        MPI_Address( &data.f1, &displs[2] );
        MPI_Address( &data.val, &displs[3] );

        types[0] = MPI_CHAR;
        types[1] = MPI_INT;
        types[2] = MPI_DOUBLE;
        types[3] = MPI_INT;

        for (i=3; i >= 0; i--)
                displs[i] -= displs[0];

        MPI_Type_struct( 4, blockcounts, displs, types, &newtype);
        MPI_Type_commit( &newtype );
        /*----------------*/

        if (myid == 0) {
		/* Fill data */
		for (i=0; i < 5; i++)
                	data.array[i] = 20 + i;
                strcpy(data.name, "hello 1234");
		data.f1 = 3.1514;  data.f2 = 0.50;
		data.val = 64;

		/* Sending */
                for (i = 1; i < numprocs; i++) {
                        MPI_Isend(&data, 1, newtype, i, 100, MPI_COMM_WORLD, &req);
			MPI_Wait (&req, &status);
			printData("--- SENT ---", i);
                        fflush(stdout);
                }
        } else {
                MPI_Irecv(&data, 1, newtype, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &req);
		MPI_Wait (&req, &status);
                printData("*** RECEIVED ***", myid);
                fflush(stdout);
        }

        MPI_Finalize();
        return 0;
}

void
printData(char *action, int rank)
{
        int i;

        printf("%s Proc #%d\t{ ", action, rank);
        for (i=0; i<10; i++) {
                printf("%c ", data.name[i]);
        }
        printf("} { ");

        for (i=0; i<5; i++) {
                printf("%d ", data.array[i]);
        }
        printf("} { ");

        printf("%f %f ", data.f1, data.f2);
        printf("} { ");

        printf("%d }", data.val);

        printf("\n");

}

