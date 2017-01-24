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
/*****************************************************************************
 *	Wrapper code for SMP compiler directives.  Translates 
 *	hypre SMP directives into the appropriate Open MP,
 *	IBM, SGI, or pgcc (Red) SMP compiler directives.
 ****************************************************************************/

#ifndef HYPRE_SMP_PRIVATE
#define HYPRE_SMP_PRIVATE
#endif

#ifdef HYPRE_USING_OPENMP
#ifndef HYPRE_SMP_REDUCTION_OP
#pragma omp parallel for private(HYPRE_SMP_PRIVATE) schedule(static)
#endif
#ifdef HYPRE_SMP_REDUCTION_OP
#pragma omp parallel for private(HYPRE_SMP_PRIVATE) \
reduction(HYPRE_SMP_REDUCTION_OP: HYPRE_SMP_REDUCTION_VARS) \
schedule(static)
#endif
#endif

#ifdef HYPRE_USING_SGI_SMP
#pragma parallel
#pragma pfor
#pragma schedtype(gss)
#pragma chunksize(10)
#endif

#ifdef HYPRE_USING_IBM_SMP
#pragma parallel_loop
#pragma schedule (guided,10)
#endif

#ifdef HYPRE_USING_PGCC_SMP
#ifndef HYPRE_SMP_REDUCTION_OP
#pragma parallel local(HYPRE_SMP_PRIVATE) pfor
#endif
#ifdef HYPRE_SMP_REDUCTION_OP
#endif
#endif

#undef HYPRE_SMP_PRIVATE
#undef HYPRE_SMP_REDUCTION_OP
#undef HYPRE_SMP_REDUCTION_VARS
