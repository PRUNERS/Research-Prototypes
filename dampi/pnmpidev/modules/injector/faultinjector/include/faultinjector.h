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

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#ifndef FAULTINJECTOR_H_
#define FAULTINJECTOR_H_

#include <mpi.h>

/*
 * -----------------------------------------------
 * Constants
 * -----------------------------------------------
*/
#define YES 1
#define NO 0

/*
 * -----------------------------------------------
 * Data corruption faults
 * -----------------------------------------------
*/
int changeToWrongValue(char *, int, MPI_Datatype);

int changeToLargerValue(char *, int, MPI_Datatype);

int changeToSmallerValue(char *, int, MPI_Datatype);

int bufferOverFlow(char *, unsigned int);

/*
 * -----------------------------------------------
 * Performance faults
 * -----------------------------------------------
*/
void infiniteLoop(void);

int finiteLoop(unsigned int);

/*
 * -----------------------------------------------
 * Communication faults
 * -----------------------------------------------
*/
int sendP2PExtraMessages(void *, int, MPI_Datatype, int, int, MPI_Comm, unsigned int);

int sendAsynchExtraMessages(void *, int, MPI_Datatype, int, int, MPI_Comm, unsigned int);

int discardMessages(void);

/*
 * -----------------------------------------------
 * General utility functions
 * -----------------------------------------------
*/
void initRand( void );

double getRandomDouble( void );

int getRandomInteger( int );

#endif /* FAULTINJECTOR_H_ */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
