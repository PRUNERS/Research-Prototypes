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
 * srefine.c
 *
 * This file contains code for the separator refinement algortihms
 *
 * Started 8/1/97
 * George
 *
 * $Id: srefine.c,v 1.1 2003/07/16 15:55:18 karypis Exp $
 *
 */

#include <metis.h>


/*************************************************************************
* This function is the entry point of the separator refinement
**************************************************************************/
void Refine2WayNode(CtrlType *ctrl, GraphType *orggraph, GraphType *graph, float ubfactor)
{

  IFSET(ctrl->dbglvl, DBG_TIME, starttimer(ctrl->UncoarsenTmr));

  for (;;) {
    IFSET(ctrl->dbglvl, DBG_TIME, starttimer(ctrl->RefTmr));
    if (ctrl->RType != 15)
      FM_2WayNodeBalance(ctrl, graph, ubfactor); 

    switch (ctrl->RType) {
      case 1:
        FM_2WayNodeRefine(ctrl, graph, ubfactor, 8); 
        break;
      case 2:
        FM_2WayNodeRefine_OneSided(ctrl, graph, ubfactor, 8); 
        break;
      case 3:
        FM_2WayNodeRefine(ctrl, graph, ubfactor, 8); 
        FM_2WayNodeRefine_OneSided(ctrl, graph, ubfactor, 8); 
        break;
      case 4:
        FM_2WayNodeRefine_OneSided(ctrl, graph, ubfactor, 8); 
        FM_2WayNodeRefine(ctrl, graph, ubfactor, 8); 
        break;
      case 5:
        FM_2WayNodeRefineEqWgt(ctrl, graph, 8); 
        break;
    }
    IFSET(ctrl->dbglvl, DBG_TIME, stoptimer(ctrl->RefTmr));

    if (graph == orggraph) 
      break;

    graph = graph->finer;
    IFSET(ctrl->dbglvl, DBG_TIME, starttimer(ctrl->ProjectTmr));
    Project2WayNodePartition(ctrl, graph);
    IFSET(ctrl->dbglvl, DBG_TIME, stoptimer(ctrl->ProjectTmr));
  }

  IFSET(ctrl->dbglvl, DBG_TIME, stoptimer(ctrl->UncoarsenTmr));
}


/*************************************************************************
* This function allocates memory for 2-way edge refinement
**************************************************************************/
void Allocate2WayNodePartitionMemory(CtrlType *ctrl, GraphType *graph)
{
  int nvtxs, pad64;

  nvtxs = graph->nvtxs;

  pad64 = (3*nvtxs+3)%2;

  graph->rdata = idxmalloc(3*nvtxs+3+(sizeof(NRInfoType)/sizeof(idxtype))*nvtxs+pad64, "Allocate2WayPartitionMemory: rdata");
  graph->pwgts          = graph->rdata;
  graph->where          = graph->rdata + 3;
  graph->bndptr         = graph->rdata + nvtxs + 3;
  graph->bndind         = graph->rdata + 2*nvtxs + 3;
  graph->nrinfo         = (NRInfoType *)(graph->rdata + 3*nvtxs + 3 + pad64);
}



/*************************************************************************
* This function computes the initial id/ed 
**************************************************************************/
void Compute2WayNodePartitionParams(CtrlType *ctrl, GraphType *graph)
{
  int i, j, k, l, nvtxs, nbnd;
  idxtype *xadj, *adjncy, *adjwgt, *vwgt;
  idxtype *where, *pwgts, *bndind, *bndptr, *edegrees;
  NRInfoType *rinfo;
  int me, other;

  nvtxs = graph->nvtxs;
  xadj = graph->xadj;
  vwgt = graph->vwgt;
  adjncy = graph->adjncy;
  adjwgt = graph->adjwgt;

  where = graph->where;
  rinfo = graph->nrinfo;
  pwgts = idxset(3, 0, graph->pwgts);
  bndind = graph->bndind;
  bndptr = idxset(nvtxs, -1, graph->bndptr);


  /*------------------------------------------------------------
  / Compute now the separator external degrees
  /------------------------------------------------------------*/
  nbnd = 0;
  for (i=0; i<nvtxs; i++) {
    me = where[i];
    pwgts[me] += vwgt[i];

    ASSERT(me >=0 && me <= 2);

    if (me == 2) { /* If it is on the separator do some computations */
      BNDInsert(nbnd, bndind, bndptr, i);

      edegrees = rinfo[i].edegrees;
      edegrees[0] = edegrees[1] = 0;

      for (j=xadj[i]; j<xadj[i+1]; j++) {
        other = where[adjncy[j]];
        if (other != 2)
          edegrees[other] += vwgt[adjncy[j]];
      }
    }
  }

  ASSERT(CheckNodeBnd(graph, nbnd));

  graph->mincut = pwgts[2];
  graph->nbnd = nbnd;
}


/*************************************************************************
* This function computes the initial id/ed 
**************************************************************************/
void Project2WayNodePartition(CtrlType *ctrl, GraphType *graph)
{
  int i, j, nvtxs;
  idxtype *cmap, *where, *cwhere;
  GraphType *cgraph;

  cgraph = graph->coarser;
  cwhere = cgraph->where;

  nvtxs = graph->nvtxs;
  cmap = graph->cmap;

  Allocate2WayNodePartitionMemory(ctrl, graph);
  where = graph->where;
  
  /* Project the partition */
  for (i=0; i<nvtxs; i++) {
    where[i] = cwhere[cmap[i]];
    ASSERTP(where[i] >= 0 && where[i] <= 2, ("%d %d %d %d\n", i, cmap[i], where[i], cwhere[cmap[i]]));
  }

  FreeGraph(graph->coarser);
  graph->coarser = NULL;

  Compute2WayNodePartitionParams(ctrl, graph);
}
