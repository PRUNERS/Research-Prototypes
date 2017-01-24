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
 * (c) 1997   The Regents of the University of California
 *
 * See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
 * notice, contact person, and disclaimer.
 *
 * $Revision: 2.0 $
 *********************************************************************EHEADER*/
/******************************************************************************
 *
 * HYPRE_StructSMG interface
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGCreate
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGCreate( MPI_Comm comm, HYPRE_StructSolver *solver )
{
   *solver = ( (HYPRE_StructSolver) hypre_SMGCreate( comm ) );

   return 0;
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGDestroy
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructSMGDestroy( HYPRE_StructSolver solver )
{
   return( hypre_SMGDestroy( (void *) solver ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetup
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructSMGSetup( HYPRE_StructSolver solver,
                      HYPRE_StructMatrix A,
                      HYPRE_StructVector b,
                      HYPRE_StructVector x      )
{
   return( hypre_SMGSetup( (void *) solver,
                           (hypre_StructMatrix *) A,
                           (hypre_StructVector *) b,
                           (hypre_StructVector *) x ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSolve
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructSMGSolve( HYPRE_StructSolver solver,
                      HYPRE_StructMatrix A,
                      HYPRE_StructVector b,
                      HYPRE_StructVector x      )
{
   return( hypre_SMGSolve( (void *) solver,
                           (hypre_StructMatrix *) A,
                           (hypre_StructVector *) b,
                           (hypre_StructVector *) x ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetMemoryUse
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetMemoryUse( HYPRE_StructSolver solver,
                             int                memory_use )
{
   return( hypre_SMGSetMemoryUse( (void *) solver, memory_use ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetTol
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetTol( HYPRE_StructSolver solver,
                       double             tol    )
{
   return( hypre_SMGSetTol( (void *) solver, tol ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetMaxIter
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetMaxIter( HYPRE_StructSolver solver,
                           int                max_iter  )
{
   return( hypre_SMGSetMaxIter( (void *) solver, max_iter ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetRelChange
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetRelChange( HYPRE_StructSolver solver,
                             int                rel_change  )
{
   return( hypre_SMGSetRelChange( (void *) solver, rel_change ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetZeroGuess
 *--------------------------------------------------------------------------*/
 
int
HYPRE_StructSMGSetZeroGuess( HYPRE_StructSolver solver )
{
   return( hypre_SMGSetZeroGuess( (void *) solver, 1 ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetNonZeroGuess
 *--------------------------------------------------------------------------*/
 
int
HYPRE_StructSMGSetNonZeroGuess( HYPRE_StructSolver solver )
{
   return( hypre_SMGSetZeroGuess( (void *) solver, 0 ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetNumPreRelax
 *
 * Note that we require at least 1 pre-relax sweep. 
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetNumPreRelax( HYPRE_StructSolver solver,
                               int                num_pre_relax )
{
   return( hypre_SMGSetNumPreRelax( (void *) solver, num_pre_relax) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetNumPostRelax
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetNumPostRelax( HYPRE_StructSolver solver,
                                int                num_post_relax )
{
   return( hypre_SMGSetNumPostRelax( (void *) solver, num_post_relax) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGSetLogging
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGSetLogging( HYPRE_StructSolver solver,
                           int                logging )
{
   return( hypre_SMGSetLogging( (void *) solver, logging) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGGetNumIterations
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGGetNumIterations( HYPRE_StructSolver  solver,
                                 int                *num_iterations )
{
   return( hypre_SMGGetNumIterations( (void *) solver, num_iterations ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructSMGGetFinalRelativeResidualNorm
 *--------------------------------------------------------------------------*/

int
HYPRE_StructSMGGetFinalRelativeResidualNorm( HYPRE_StructSolver  solver,
                                             double             *norm   )
{
   return( hypre_SMGGetFinalRelativeResidualNorm( (void *) solver, norm ) );
}
