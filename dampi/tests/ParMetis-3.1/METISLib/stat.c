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
 * stat.c
 *
 * This file computes various statistics
 *
 * Started 7/25/97
 * George
 *
 * $Id: stat.c,v 1.2 2003/07/24 18:39:12 karypis Exp $
 *
 */

#include <metis.h>


/*************************************************************************
* This function computes cuts and balance information
**************************************************************************/
void ComputePartitionInfo(GraphType *graph, int nparts, idxtype *where)
{
  int i, j, k, nvtxs, ncon, mustfree=0;
  idxtype *xadj, *adjncy, *vwgt, *adjwgt, *kpwgts, *tmpptr;
  idxtype *padjncy, *padjwgt, *padjcut;

  nvtxs = graph->nvtxs;
  ncon = graph->ncon;
  xadj = graph->xadj;
  adjncy = graph->adjncy;
  vwgt = graph->vwgt;
  adjwgt = graph->adjwgt;

  if (vwgt == NULL) {
    vwgt = graph->vwgt = idxsmalloc(nvtxs, 1, "vwgt");
    mustfree = 1;
  }
  if (adjwgt == NULL) {
    adjwgt = graph->adjwgt = idxsmalloc(xadj[nvtxs], 1, "adjwgt");
    mustfree += 2;
  }

  printf("%d-way Cut: %5d, Vol: %5d, ", nparts, ComputeCut(graph, where), ComputeVolume(graph, where));

  /* Compute balance information */
  kpwgts = idxsmalloc(ncon*nparts, 0, "ComputePartitionInfo: kpwgts");

  for (i=0; i<nvtxs; i++) {
    for (j=0; j<ncon; j++) 
      kpwgts[where[i]*ncon+j] += vwgt[i*ncon+j];
  }

  if (ncon == 1) {
    printf("\tBalance: %5.3f out of %5.3f\n", 
            1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)),
            1.0*nparts*vwgt[idxamax(nvtxs, vwgt)]/(1.0*idxsum(nparts, kpwgts)));
  }
  else {
    printf("\tBalance:");
    for (j=0; j<ncon; j++) 
      printf(" (%5.3f out of %5.3f)", 
            1.0*nparts*kpwgts[ncon*idxamax_strd(nparts, kpwgts+j, ncon)+j]/(1.0*idxsum_strd(nparts, kpwgts+j, ncon)),
            1.0*nparts*vwgt[ncon*idxamax_strd(nvtxs, vwgt+j, ncon)+j]/(1.0*idxsum_strd(nparts, kpwgts+j, ncon)));
    printf("\n");
  }


  /* Compute p-adjncy information */
  padjncy = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjncy");
  padjwgt = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjwgt");
  padjcut = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjwgt");

  idxset(nparts, 0, kpwgts);
  for (i=0; i<nvtxs; i++) {
    for (j=xadj[i]; j<xadj[i+1]; j++) {
      if (where[i] != where[adjncy[j]]) {
        padjncy[where[i]*nparts+where[adjncy[j]]] = 1;
        padjcut[where[i]*nparts+where[adjncy[j]]] += adjwgt[j];
        if (kpwgts[where[adjncy[j]]] == 0) {
          padjwgt[where[i]*nparts+where[adjncy[j]]]++;
          kpwgts[where[adjncy[j]]] = 1;
        }
      }
    }
    for (j=xadj[i]; j<xadj[i+1]; j++) 
      kpwgts[where[adjncy[j]]] = 0;
  }

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjncy+i*nparts);
  printf("Min/Max/Avg/Bal # of adjacent     subdomains: %5d %5d %5.2f %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], 
    1.0*idxsum(nparts, kpwgts)/(1.0*nparts), 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)));

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjcut+i*nparts);
  printf("Min/Max/Avg/Bal # of adjacent subdomain cuts: %5d %5d %5d %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], idxsum(nparts, kpwgts)/nparts, 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)));

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjwgt+i*nparts);
  printf("Min/Max/Avg/Bal/Frac # of interface    nodes: %5d %5d %5d %7.3f %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], idxsum(nparts, kpwgts)/nparts, 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)), 1.0*idxsum(nparts, kpwgts)/(1.0*nvtxs));

  tmpptr = graph->where;
  graph->where = where;
  for (i=0; i<nparts; i++)
    IsConnectedSubdomain(NULL, graph, i, 1);
  graph->where = tmpptr;

  if (mustfree == 1 || mustfree == 3) {
    free(vwgt);
    graph->vwgt = NULL;
  }
  if (mustfree == 2 || mustfree == 3) {
    free(adjwgt);
    graph->adjwgt = NULL;
  }

  GKfree(&kpwgts, &padjncy, &padjwgt, &padjcut, LTERM);
}


/*************************************************************************
* This function computes cuts and balance information
**************************************************************************/
void ComputePartitionInfoBipartite(GraphType *graph, int nparts, idxtype *where)
{
  int i, j, k, nvtxs, ncon, mustfree=0;
  idxtype *xadj, *adjncy, *vwgt, *vsize, *adjwgt, *kpwgts, *tmpptr;
  idxtype *padjncy, *padjwgt, *padjcut;

  nvtxs = graph->nvtxs;
  ncon = graph->ncon;
  xadj = graph->xadj;
  adjncy = graph->adjncy;
  vwgt = graph->vwgt;
  vsize = graph->vsize;
  adjwgt = graph->adjwgt;

  if (vwgt == NULL) {
    vwgt = graph->vwgt = idxsmalloc(nvtxs, 1, "vwgt");
    mustfree = 1;
  }
  if (adjwgt == NULL) {
    adjwgt = graph->adjwgt = idxsmalloc(xadj[nvtxs], 1, "adjwgt");
    mustfree += 2;
  }

  printf("%d-way Cut: %5d, Vol: %5d, ", nparts, ComputeCut(graph, where), ComputeVolume(graph, where));

  /* Compute balance information */
  kpwgts = idxsmalloc(ncon*nparts, 0, "ComputePartitionInfo: kpwgts");

  for (i=0; i<nvtxs; i++) {
    for (j=0; j<ncon; j++) 
      kpwgts[where[i]*ncon+j] += vwgt[i*ncon+j];
  }

  if (ncon == 1) {
    printf("\tBalance: %5.3f out of %5.3f\n", 
            1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)),
            1.0*nparts*vwgt[idxamax(nvtxs, vwgt)]/(1.0*idxsum(nparts, kpwgts)));
  }
  else {
    printf("\tBalance:");
    for (j=0; j<ncon; j++) 
      printf(" (%5.3f out of %5.3f)", 
            1.0*nparts*kpwgts[ncon*idxamax_strd(nparts, kpwgts+j, ncon)+j]/(1.0*idxsum_strd(nparts, kpwgts+j, ncon)),
            1.0*nparts*vwgt[ncon*idxamax_strd(nvtxs, vwgt+j, ncon)+j]/(1.0*idxsum_strd(nparts, kpwgts+j, ncon)));
    printf("\n");
  }


  /* Compute p-adjncy information */
  padjncy = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjncy");
  padjwgt = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjwgt");
  padjcut = idxsmalloc(nparts*nparts, 0, "ComputePartitionInfo: padjwgt");

  idxset(nparts, 0, kpwgts);
  for (i=0; i<nvtxs; i++) {
    for (j=xadj[i]; j<xadj[i+1]; j++) {
      if (where[i] != where[adjncy[j]]) {
        padjncy[where[i]*nparts+where[adjncy[j]]] = 1;
        padjcut[where[i]*nparts+where[adjncy[j]]] += adjwgt[j];
        if (kpwgts[where[adjncy[j]]] == 0) {
          padjwgt[where[i]*nparts+where[adjncy[j]]] += vsize[i];
          kpwgts[where[adjncy[j]]] = 1;
        }
      }
    }
    for (j=xadj[i]; j<xadj[i+1]; j++) 
      kpwgts[where[adjncy[j]]] = 0;
  }

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjncy+i*nparts);
  printf("Min/Max/Avg/Bal # of adjacent     subdomains: %5d %5d %5d %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], idxsum(nparts, kpwgts)/nparts, 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)));

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjcut+i*nparts);
  printf("Min/Max/Avg/Bal # of adjacent subdomain cuts: %5d %5d %5d %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], idxsum(nparts, kpwgts)/nparts, 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)));

  for (i=0; i<nparts; i++)
    kpwgts[i] = idxsum(nparts, padjwgt+i*nparts);
  printf("Min/Max/Avg/Bal/Frac # of interface    nodes: %5d %5d %5d %7.3f %7.3f\n",
    kpwgts[idxamin(nparts, kpwgts)], kpwgts[idxamax(nparts, kpwgts)], idxsum(nparts, kpwgts)/nparts, 
    1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts)), 1.0*idxsum(nparts, kpwgts)/(1.0*nvtxs));


  if (mustfree == 1 || mustfree == 3) {
    free(vwgt);
    graph->vwgt = NULL;
  }
  if (mustfree == 2 || mustfree == 3) {
    free(adjwgt);
    graph->adjwgt = NULL;
  }

  GKfree(&kpwgts, &padjncy, &padjwgt, &padjcut, LTERM);
}



/*************************************************************************
* This function computes the balance of the partitioning
**************************************************************************/
void ComputePartitionBalance(GraphType *graph, int nparts, idxtype *where, float *ubvec)
{
  int i, j, nvtxs, ncon;
  idxtype *kpwgts, *vwgt;
  float balance;

  nvtxs = graph->nvtxs;
  ncon = graph->ncon;
  vwgt = graph->vwgt;

  kpwgts = idxsmalloc(nparts, 0, "ComputePartitionInfo: kpwgts");

  if (vwgt == NULL && ncon == 1) {
    for (i=0; i<nvtxs; i++)
      kpwgts[where[i]]++;
    ubvec[0] = 1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*nvtxs);
  }
  else {
    for (j=0; j<ncon; j++) {
      idxset(nparts, 0, kpwgts);
      for (i=0; i<graph->nvtxs; i++)
        kpwgts[where[i]] += vwgt[i*ncon+j];

      ubvec[j] = 1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts));
    }
  }

  free(kpwgts);

}


/*************************************************************************
* This function computes the balance of the element partitioning
**************************************************************************/
float ComputeElementBalance(int ne, int nparts, idxtype *where)
{
  int i;
  idxtype *kpwgts;
  float balance;

  kpwgts = idxsmalloc(nparts, 0, "ComputeElementBalance: kpwgts");

  for (i=0; i<ne; i++)
    kpwgts[where[i]]++;

  balance = 1.0*nparts*kpwgts[idxamax(nparts, kpwgts)]/(1.0*idxsum(nparts, kpwgts));

  free(kpwgts);

  return balance;

}


/*************************************************************************
* This function computes the balance of the partitioning
**************************************************************************/
void Moc_ComputePartitionBalance(GraphType *graph, int nparts, idxtype *where, float *ubvec)
{
  int i, j, nvtxs, ncon;
  float *kpwgts, *nvwgt;
  float balance;

  nvtxs = graph->nvtxs;
  ncon = graph->ncon;
  nvwgt = graph->nvwgt;

  kpwgts = fmalloc(nparts, "ComputePartitionInfo: kpwgts");

  for (j=0; j<ncon; j++) {
    sset(nparts, 0.0, kpwgts);
    for (i=0; i<graph->nvtxs; i++)
      kpwgts[where[i]] += nvwgt[i*ncon+j];

    ubvec[j] = (float)nparts*kpwgts[samax(nparts, kpwgts)]/ssum(nparts, kpwgts);
  }

  free(kpwgts);

}

