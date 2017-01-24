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

/*
 * Copyright 1997, Regents of the University of Minnesota
 *
 * csrmatch.c
 *
 * This file contains the code that computes matchings
 *
 * Started 7/23/97
 * George
 *
 * $Id: csrmatch.c,v 1.2 2003/07/21 17:18:48 karypis Exp $
 *
 */

#include <parmetislib.h>




/*************************************************************************
* This function finds a matching using the HEM heuristic
**************************************************************************/
void CSR_Match_SHEM(MatrixType *matrix, idxtype *match, idxtype *mlist,
     idxtype *skip, int ncon)
{
  int h, i, ii, j;
  int nrows, edge, maxidx, count;
  float maxwgt;
  idxtype *rowptr, *colind;
  float *transfer;
  KVType *links;

  nrows = matrix->nrows;
  rowptr = matrix->rowptr;
  colind = matrix->colind;
  transfer = matrix->transfer;

  idxset(nrows, UNMATCHED, match);

  links = (KVType *)GKmalloc(sizeof(KVType)*nrows, "links");
  for (i=0; i<nrows; i++) { 
    links[i].key = i; 
    links[i].val = 0.0;
  }

  for (i=0; i<nrows; i++)
    for (j=rowptr[i]; j<rowptr[i+1]; j++) 
      for (h=0; h<ncon; h++)
        if (links[i].val < fabs(transfer[j*ncon+h]))
          links[i].val = fabs(transfer[j*ncon+h]);

  qsort(links, nrows, sizeof(KVType), myvalkeycompare);

  count = 0;
  for (ii=0; ii<nrows; ii++) {
    i = links[ii].key;

    if (match[i] == UNMATCHED) {
      maxidx = i;
      maxwgt = 0.0;

      /* Find a heavy-edge matching */
      for (j=rowptr[i]; j<rowptr[i+1]; j++) {
        edge = colind[j];
        if (match[edge] == UNMATCHED && edge != i && skip[j] == 0) {
          for (h=0; h<ncon; h++)
            if (maxwgt < fabs(transfer[j*ncon+h]))
              break;

          if (h != ncon) {
            maxwgt = fabs(transfer[j*ncon+h]);
            maxidx = edge;
          }
        }
      }

      if (maxidx != i) {
        match[i] = maxidx;
        match[maxidx] = i;
        mlist[count++] = amax(i, maxidx);
        mlist[count++] = amin(i, maxidx);
      }
    }
  }

  GKfree((void **)&links, LTERM);
}

