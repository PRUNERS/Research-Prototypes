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
 * Copyright (c) 2006   The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the HYPRE team. UCRL-CODE-222953.
 * All rights reserved.
 *
 * This file is part of HYPRE (see http://www.llnl.gov/CASC/hypre/).
 * Please see the COPYRIGHT_and_LICENSE file for the copyright notice, 
 * disclaimer, contact information and the GNU Lesser General Public License.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the 
 * terms of the GNU General Public License (as published by the Free Software
 * Foundation) version 2.1 dated February 1999.
 *
 * HYPRE is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE.  See the terms and conditions of the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Revision: 2.3 $
 ***********************************************************************EHEADER*/



/******************************************************************************
 *
 * Header file for HYPRE_parcsr_mv library
 *
 *****************************************************************************/

#ifndef HYPRE_PARCSR_MV_HEADER
#define HYPRE_PARCSR_MV_HEADER

#include "HYPRE_utilities.h"
#include "HYPRE_seq_mv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------
 * Structures
 *--------------------------------------------------------------------------*/

struct hypre_ParCSRMatrix_struct;
typedef struct hypre_ParCSRMatrix_struct *HYPRE_ParCSRMatrix;
struct hypre_ParVector_struct;
typedef struct hypre_ParVector_struct *HYPRE_ParVector;

/*--------------------------------------------------------------------------
 * Prototypes
 *--------------------------------------------------------------------------*/

/* HYPRE_parcsr_matrix.c */
int HYPRE_ParCSRMatrixCreate( MPI_Comm comm , HYPRE_BigInt global_num_rows , HYPRE_BigInt global_num_cols , HYPRE_BigInt *row_starts , HYPRE_BigInt *col_starts , int num_cols_offd , int num_nonzeros_diag , int num_nonzeros_offd , HYPRE_ParCSRMatrix *matrix );
int HYPRE_ParCSRMatrixDestroy( HYPRE_ParCSRMatrix matrix );
int HYPRE_ParCSRMatrixInitialize( HYPRE_ParCSRMatrix matrix );
int HYPRE_ParCSRMatrixRead( MPI_Comm comm , const char *file_name , HYPRE_ParCSRMatrix *matrix );
int HYPRE_ParCSRMatrixPrint( HYPRE_ParCSRMatrix matrix , const char *file_name );
int HYPRE_ParCSRMatrixPrintIJ( HYPRE_ParCSRMatrix matrix , int base_i, int bse_j, const char *file_name );
int HYPRE_ParCSRMatrixGetComm( HYPRE_ParCSRMatrix matrix , MPI_Comm *comm );
int HYPRE_ParCSRMatrixGetDims( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt *M , HYPRE_BigInt *N );
int HYPRE_ParCSRMatrixGetRowPartitioning( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt **row_partitioning_ptr );
int HYPRE_ParCSRMatrixGetColPartitioning( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt **col_partitioning_ptr );
int HYPRE_ParCSRMatrixGetLocalRange( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt *row_start , HYPRE_BigInt *row_end , HYPRE_BigInt *col_start , HYPRE_BigInt *col_end );
int HYPRE_ParCSRMatrixGetRow( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt row , int *size , HYPRE_BigInt **col_ind , double **values );
int HYPRE_ParCSRMatrixRestoreRow( HYPRE_ParCSRMatrix matrix , HYPRE_BigInt row , int *size , HYPRE_BigInt **col_ind , double **values );
int HYPRE_CSRMatrixToParCSRMatrix( MPI_Comm comm , HYPRE_CSRMatrix A_CSR , HYPRE_BigInt *row_partitioning , HYPRE_BigInt *col_partitioning , HYPRE_ParCSRMatrix *matrix );
int HYPRE_ParCSRMatrixMatvec( double alpha , HYPRE_ParCSRMatrix A , HYPRE_ParVector x , double beta , HYPRE_ParVector y );
int HYPRE_ParCSRMatrixMatvecT( double alpha , HYPRE_ParCSRMatrix A , HYPRE_ParVector x , double beta , HYPRE_ParVector y );

/* HYPRE_parcsr_vector.c */
int HYPRE_ParVectorCreate( MPI_Comm comm , HYPRE_BigInt global_size , HYPRE_BigInt *partitioning , HYPRE_ParVector *vector );
int HYPRE_ParVectorDestroy( HYPRE_ParVector vector );
int HYPRE_ParVectorInitialize( HYPRE_ParVector vector );
int HYPRE_ParVectorRead( MPI_Comm comm , const char *file_name , HYPRE_ParVector *vector );
int HYPRE_ParVectorPrint( HYPRE_ParVector vector , const char *file_name );
int HYPRE_ParVectorPrintIJ( HYPRE_ParVector vector , int base_i, const char *file_name );
int HYPRE_ParVectorSetConstantValues( HYPRE_ParVector vector , double value );
int HYPRE_ParVectorSetRandomValues( HYPRE_ParVector vector , int seed );
int HYPRE_ParVectorCopy( HYPRE_ParVector x , HYPRE_ParVector y );
int HYPRE_ParVectorScale( double value , HYPRE_ParVector x );
int HYPRE_ParVectorInnerProd( HYPRE_ParVector x , HYPRE_ParVector y , double *prod );
int HYPRE_VectorToParVector( MPI_Comm comm , HYPRE_Vector b , HYPRE_BigInt *partitioning , HYPRE_ParVector *vector );

#ifdef __cplusplus
}
#endif

#endif

