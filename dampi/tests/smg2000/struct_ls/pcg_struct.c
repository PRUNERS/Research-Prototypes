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

/*BHEADER**********************************************************************
 * (c) 1998   The Regents of the University of California
 *
 * See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
 * notice, contact person, and disclaimer.
 *
 * $Revision: 2.0 $
 *********************************************************************EHEADER*/

/******************************************************************************
 *
 * Struct matrix-vector implementation of PCG interface routines.
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * hypre_StructKrylovCAlloc
 *--------------------------------------------------------------------------*/

char *
hypre_StructKrylovCAlloc( int count,
                 int elt_size )
{
   return( hypre_CAlloc( count, elt_size ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovFree
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovFree( char *ptr )
{
   int ierr = 0;

   hypre_Free( ptr );

   return ierr;
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovCreateVector
 *--------------------------------------------------------------------------*/

void *
hypre_StructKrylovCreateVector( void *vvector )
{
   hypre_StructVector *vector = vvector;
   hypre_StructVector *new_vector;

   new_vector = hypre_StructVectorCreate( hypre_StructVectorComm(vector),
                                          hypre_StructVectorGrid(vector) );
   hypre_StructVectorInitialize(new_vector);
   hypre_StructVectorAssemble(new_vector);

   return ( (void *) new_vector );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovCreateVectorArray
 *--------------------------------------------------------------------------*/

void *
hypre_StructKrylovCreateVectorArray(int n, void *vvector )
{
   hypre_StructVector *vector = vvector;
   hypre_StructVector **new_vector;
   int i;

   new_vector = hypre_CTAlloc(hypre_StructVector*,n);
   for (i=0; i < n; i++)
   {
      HYPRE_StructVectorCreate(hypre_StructVectorComm(vector),
                                hypre_StructVectorGrid(vector),
                                (HYPRE_StructVector *) &new_vector[i] );
      HYPRE_StructVectorInitialize((HYPRE_StructVector) new_vector[i]);
      HYPRE_StructVectorAssemble((HYPRE_StructVector) new_vector[i]);
   }

   return ( (void *) new_vector );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovDestroyVector
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovDestroyVector( void *vvector )
{
   hypre_StructVector *vector = vvector;

   return( hypre_StructVectorDestroy( vector ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovMatvecCreate
 *--------------------------------------------------------------------------*/

void *
hypre_StructKrylovMatvecCreate( void   *A,
                       void   *x )
{
   void *matvec_data;

   matvec_data = hypre_StructMatvecCreate();
   hypre_StructMatvecSetup(matvec_data, A, x);

   return ( matvec_data );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovMatvec
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovMatvec( void   *matvec_data,
                 double  alpha,
                 void   *A,
                 void   *x,
                 double  beta,
                 void   *y           )
{
   return ( hypre_StructMatvecCompute( matvec_data,
                                       alpha,
                                       (hypre_StructMatrix *) A,
                                       (hypre_StructVector *) x,
                                       beta,
                                       (hypre_StructVector *) y ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovMatvecDestroy
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovMatvecDestroy( void *matvec_data )
{
   return ( hypre_StructMatvecDestroy( matvec_data ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovInnerProd
 *--------------------------------------------------------------------------*/

double
hypre_StructKrylovInnerProd( void *x, 
                    void *y )
{
   return ( hypre_StructInnerProd( (hypre_StructVector *) x,
                                   (hypre_StructVector *) y ) );
}


/*--------------------------------------------------------------------------
 * hypre_StructKrylovCopyVector
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovCopyVector( void *x, 
                     void *y )
{
   return ( hypre_StructCopy( (hypre_StructVector *) x,
                              (hypre_StructVector *) y ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovClearVector
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovClearVector( void *x )
{
   return ( hypre_StructVectorSetConstantValues( (hypre_StructVector *) x,
                                                 0.0 ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovScaleVector
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovScaleVector( double  alpha,
                      void   *x     )
{
   return ( hypre_StructScale( alpha, (hypre_StructVector *) x ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovAxpy
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovAxpy( double alpha,
               void   *x,
               void   *y )
{
   return ( hypre_StructAxpy( alpha, (hypre_StructVector *) x,
                              (hypre_StructVector *) y ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovIdentitySetup (for a default preconditioner)
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovIdentitySetup( void *vdata,
                        void *A,
                        void *b,
                        void *x     )

{
   return 0;
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovIdentity (for a default preconditioner)
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovIdentity( void *vdata,
                   void *A,
                   void *b,
                   void *x     )

{
   return( hypre_StructKrylovCopyVector( b, x ) );
}

/*--------------------------------------------------------------------------
 * hypre_StructKrylovCommInfo
 *--------------------------------------------------------------------------*/

int
hypre_StructKrylovCommInfo( void  *A,
                      int   *my_id,
                      int   *num_procs )
{
   MPI_Comm comm = hypre_StructMatrixComm((hypre_StructMatrix *) A);
   MPI_Comm_size(comm,num_procs);
   MPI_Comm_rank(comm,my_id);
   return 0;
}

