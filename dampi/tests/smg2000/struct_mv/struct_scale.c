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
 * Structured scale routine
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * hypre_StructScale
 *--------------------------------------------------------------------------*/

int
hypre_StructScale( double              alpha,
                   hypre_StructVector *y     )
{
   int ierr = 0;

   hypre_Box       *y_data_box;
                   
   int              yi;
   double          *yp;
                   
   hypre_BoxArray  *boxes;
   hypre_Box       *box;
   hypre_Index      loop_size;
   hypre_IndexRef   start;
   hypre_Index      unit_stride;
                   
   int              i;
   int              loopi, loopj, loopk;

   hypre_SetIndex(unit_stride, 1, 1, 1);

   boxes = hypre_StructGridBoxes(hypre_StructVectorGrid(y));
   hypre_ForBoxI(i, boxes)
      {
         box   = hypre_BoxArrayBox(boxes, i);
         start = hypre_BoxIMin(box);

         y_data_box = hypre_BoxArrayBox(hypre_StructVectorDataSpace(y), i);
         yp = hypre_StructVectorBoxData(y, i);

         hypre_BoxGetSize(box, loop_size);

	 hypre_BoxLoop1Begin(loop_size,
                             y_data_box, start, unit_stride, yi);
#define HYPRE_BOX_SMP_PRIVATE loopk,loopi,loopj,yi
#include "hypre_box_smp_forloop.h"
	 hypre_BoxLoop1For(loopi, loopj, loopk, yi)
            {
               yp[yi] *= alpha;
            }
	 hypre_BoxLoop1End(yi);
      }

   return ierr;
}
