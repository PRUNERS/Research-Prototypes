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
 * (c) 1996   The Regents of the University of California
 *
 * See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
 * notice, contact person, and disclaimer.
 *
 * $Revision: 2.0 $
 *********************************************************************EHEADER*/

/******************************************************************************
 *
 * Routines for generating random numbers.
 *
 *****************************************************************************/


/*--------------------------------------------------------------------------
 * Static variables
 *--------------------------------------------------------------------------*/

static int Seed = 13579;

#define L 1664525
#define M 1024

/*--------------------------------------------------------------------------
 * hypre_SeedRand:
 *   The seed must always be positive.
 *
 *   Note: the internal seed must be positive and odd, so it is set
 *   to (2*input_seed - 1);
 *--------------------------------------------------------------------------*/

void  hypre_SeedRand(seed)
int   seed;
{
   Seed = (2*seed - 1) % M;
}

/*--------------------------------------------------------------------------
 * hypre_Rand
 *--------------------------------------------------------------------------*/

double  hypre_Rand()
{
   Seed = (L * Seed) % M;

   return ( ((double) Seed) / ((double) M) );
}
