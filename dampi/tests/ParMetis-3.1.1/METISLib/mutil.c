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
 * mutil.c 
 *
 * This file contains various utility functions for the MOC portion of the
 * code
 *
 * Started 2/15/98
 * George
 *
 * $Id: mutil.c,v 1.1 2003/07/16 15:55:13 karypis Exp $
 *
 */

#include <metis.h>


/*************************************************************************
* This function checks if the vertex weights of two vertices are below 
* a given set of values
**************************************************************************/
int AreAllVwgtsBelow(int ncon, float alpha, float *vwgt1, float beta, float *vwgt2, float limit)
{
  int i;

  for (i=0; i<ncon; i++)
    if (alpha*vwgt1[i] + beta*vwgt2[i] > limit)
      return 0;

  return 1;
}


/*************************************************************************
* This function checks if the vertex weights of two vertices are below 
* a given set of values
**************************************************************************/
int AreAnyVwgtsBelow(int ncon, float alpha, float *vwgt1, float beta, float *vwgt2, float limit)
{
  int i;

  for (i=0; i<ncon; i++)
    if (alpha*vwgt1[i] + beta*vwgt2[i] < limit)
      return 1;

  return 0;
}



/*************************************************************************
* This function checks if the vertex weights of two vertices are above 
* a given set of values
**************************************************************************/
int AreAllVwgtsAbove(int ncon, float alpha, float *vwgt1, float beta, float *vwgt2, float limit)
{
  int i;

  for (i=0; i<ncon; i++)
    if (alpha*vwgt1[i] + beta*vwgt2[i] < limit)
      return 0;

  return 1;
}


/*************************************************************************
* This function computes the load imbalance over all the constrains
* For now assume that we just want balanced partitionings
**************************************************************************/ 
float ComputeLoadImbalance(int ncon, int nparts, float *npwgts, float *tpwgts)
{
  int i, j;
  float max, lb=0.0;

  for (i=0; i<ncon; i++) {
    max = 0.0;
    for (j=0; j<nparts; j++) {
      if (npwgts[j*ncon+i] > max)
        max = npwgts[j*ncon+i];
    }
    if (max*nparts > lb)
      lb = max*nparts;
  }

  return lb;
}

/*************************************************************************
* This function checks if the vertex weights of two vertices are below 
* a given set of values
**************************************************************************/
int AreAllBelow(int ncon, float *v1, float *v2)
{
  int i;

  for (i=0; i<ncon; i++)
    if (v1[i] > v2[i])
      return 0;

  return 1;
}
