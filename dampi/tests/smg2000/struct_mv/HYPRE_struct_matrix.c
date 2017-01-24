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
 * HYPRE_StructMatrix interface
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixCreate
 *--------------------------------------------------------------------------*/

int
HYPRE_StructMatrixCreate( MPI_Comm             comm,
                          HYPRE_StructGrid     grid,
                          HYPRE_StructStencil  stencil,
                          HYPRE_StructMatrix  *matrix )
{
   *matrix = hypre_StructMatrixCreate(comm, grid, stencil);

   return 0;
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixDestroy
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixDestroy( HYPRE_StructMatrix matrix )
{
   return( hypre_StructMatrixDestroy(matrix) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixInitialize
 *--------------------------------------------------------------------------*/

int
HYPRE_StructMatrixInitialize( HYPRE_StructMatrix matrix )
{
   return ( hypre_StructMatrixInitialize(matrix) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixSetValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixSetValues( HYPRE_StructMatrix  matrix,
                             int                *grid_index,
                             int                 num_stencil_indices,
                             int                *stencil_indices,
                             double             *values )
{
   hypre_Index  new_grid_index;
                
   int          d;
   int          ierr = 0;

   hypre_ClearIndex(new_grid_index);
   for (d = 0; d < hypre_StructGridDim(hypre_StructMatrixGrid(matrix)); d++)
   {
      hypre_IndexD(new_grid_index, d) = grid_index[d];
   }

   ierr = hypre_StructMatrixSetValues(matrix, new_grid_index,
                                      num_stencil_indices, stencil_indices,
                                      values, 0);

   return (ierr);
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixSetBoxValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixSetBoxValues( HYPRE_StructMatrix  matrix,
                                int                *ilower,
                                int                *iupper,
                                int                 num_stencil_indices,
                                int                *stencil_indices,
                                double             *values )
{
   hypre_Index         new_ilower;
   hypre_Index         new_iupper;
   hypre_Box          *new_value_box;
                    
   int                 d;
   int                 ierr = 0;

   hypre_ClearIndex(new_ilower);
   hypre_ClearIndex(new_iupper);
   for (d = 0; d < hypre_StructGridDim(hypre_StructMatrixGrid(matrix)); d++)
   {
      hypre_IndexD(new_ilower, d) = ilower[d];
      hypre_IndexD(new_iupper, d) = iupper[d];
   }
   new_value_box = hypre_BoxCreate();
   hypre_BoxSetExtents(new_value_box, new_ilower, new_iupper);

   ierr = hypre_StructMatrixSetBoxValues(matrix, new_value_box,
                                         num_stencil_indices, stencil_indices,
                                         values, 0);

   hypre_BoxDestroy(new_value_box);

   return (ierr);
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixAddToValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixAddToValues( HYPRE_StructMatrix  matrix,
                               int                *grid_index,
                               int                 num_stencil_indices,
                               int                *stencil_indices,
                               double             *values )
{
   hypre_Index         new_grid_index;

   int                 d;
   int                 ierr = 0;

   hypre_ClearIndex(new_grid_index);
   for (d = 0; d < hypre_StructGridDim(hypre_StructMatrixGrid(matrix)); d++)
   {
      hypre_IndexD(new_grid_index, d) = grid_index[d];
   }

   ierr = hypre_StructMatrixSetValues(matrix, new_grid_index,
                                      num_stencil_indices, stencil_indices,
                                      values, 1);

   return (ierr);
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixAddToBoxValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixAddToBoxValues( HYPRE_StructMatrix  matrix,
                                  int                *ilower,
                                  int                *iupper,
                                  int                 num_stencil_indices,
                                  int                *stencil_indices,
                                  double             *values )
{
   hypre_Index         new_ilower;
   hypre_Index         new_iupper;
   hypre_Box          *new_value_box;
                    
   int                 d;
   int                 ierr = 0;

   hypre_ClearIndex(new_ilower);
   hypre_ClearIndex(new_iupper);
   for (d = 0; d < hypre_StructGridDim(hypre_StructMatrixGrid(matrix)); d++)
   {
      hypre_IndexD(new_ilower, d) = ilower[d];
      hypre_IndexD(new_iupper, d) = iupper[d];
   }
   new_value_box = hypre_BoxCreate();
   hypre_BoxSetExtents(new_value_box, new_ilower, new_iupper);

   ierr = hypre_StructMatrixSetBoxValues(matrix, new_value_box,
                                         num_stencil_indices, stencil_indices,
                                         values, 1);

   hypre_BoxDestroy(new_value_box);

   return (ierr);
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixAssemble
 *--------------------------------------------------------------------------*/

int 
HYPRE_StructMatrixAssemble( HYPRE_StructMatrix matrix )
{
   return( hypre_StructMatrixAssemble(matrix) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixSetNumGhost
 *--------------------------------------------------------------------------*/
 
int
HYPRE_StructMatrixSetNumGhost( HYPRE_StructMatrix  matrix,
                               int                *num_ghost )
{
   return ( hypre_StructMatrixSetNumGhost(matrix, num_ghost) );
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixGetGrid
 *--------------------------------------------------------------------------*/

int
HYPRE_StructMatrixGetGrid( HYPRE_StructMatrix matrix, HYPRE_StructGrid *grid )
{
   int ierr = 0;

   *grid = hypre_StructMatrixGrid(matrix);

   return ierr;
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixSetSymmetric
 *--------------------------------------------------------------------------*/
 
int
HYPRE_StructMatrixSetSymmetric( HYPRE_StructMatrix  matrix,
                                int                 symmetric )
{
   int ierr  = 0;

   hypre_StructMatrixSymmetric(matrix) = symmetric;

   return ierr;
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixPrint
 *--------------------------------------------------------------------------*/

int
HYPRE_StructMatrixPrint( char               *filename,
                         HYPRE_StructMatrix  matrix,
                         int                 all )
{
   return ( hypre_StructMatrixPrint(filename, matrix, all) );
}
