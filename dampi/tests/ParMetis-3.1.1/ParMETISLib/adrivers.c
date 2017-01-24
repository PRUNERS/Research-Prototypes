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
 * adrivers.c
 *
 * This file contains the driving routines for the various parallel
 * multilevel partitioning and repartitioning algorithms
 *
 * Started 11/19/96
 * George
 *
 * $Id: adrivers.c,v 1.5 2003/07/30 18:37:58 karypis Exp $
 *
 */

#include <parmetislib.h>



/*************************************************************************
* This function is the driver for the adaptive refinement mode of ParMETIS
**************************************************************************/
void Adaptive_Partition(CtrlType *ctrl, GraphType *graph, WorkSpaceType *wspace)
{
  int i;
  int tewgt, tvsize;
  float gtewgt, gtvsize;
  float ubavg, lbavg, lbvec[MAXNCON];

  /************************************/
  /* Set up important data structures */
  /************************************/
  SetUp(ctrl, graph, wspace);

  ubavg   = savg(graph->ncon, ctrl->ubvec);
  tewgt   = idxsum(graph->nedges, graph->adjwgt);
  tvsize  = idxsum(graph->nvtxs, graph->vsize);
  gtewgt  = (float) GlobalSESum(ctrl, tewgt) + 1.0;  /* The +1 were added to remove any FPE */
  gtvsize = (float) GlobalSESum(ctrl, tvsize) + 1.0;
  ctrl->redist_factor = ctrl->redist_base * ((gtewgt/gtvsize)/ ctrl->edge_size_ratio);

  IFSET(ctrl->dbglvl, DBG_PROGRESS, rprintf(ctrl, "[%6d %8d %5d %5d][%d]\n", 
        graph->gnvtxs, GlobalSESum(ctrl, graph->nedges), GlobalSEMin(ctrl, graph->nvtxs), GlobalSEMax(ctrl, graph->nvtxs), ctrl->CoarsenTo));

  if (graph->gnvtxs < 1.3*ctrl->CoarsenTo ||
     (graph->finer != NULL && graph->gnvtxs > graph->finer->gnvtxs*COARSEN_FRACTION)) {

    /***********************************************/
    /* Balance the partition on the coarsest graph */
    /***********************************************/
    graph->where = idxsmalloc(graph->nvtxs+graph->nrecv, -1, "graph->where");
    idxcopy(graph->nvtxs, graph->home, graph->where);

    Mc_ComputeParallelBalance(ctrl, graph, graph->where, lbvec);
    lbavg = savg(graph->ncon, lbvec);

    if (lbavg > ubavg + 0.035 && ctrl->partType != REFINE_PARTITION)
      Balance_Partition(ctrl, graph, wspace);

    if (ctrl->dbglvl&DBG_PROGRESS) {
      Mc_ComputeParallelBalance(ctrl, graph, graph->where, lbvec);
      rprintf(ctrl, "nvtxs: %10d, balance: ", graph->gnvtxs);
      for (i=0; i<graph->ncon; i++) 
        rprintf(ctrl, "%.3f ", lbvec[i]);
      rprintf(ctrl, "\n");
    }

    /* check if no coarsening took place */
    if (graph->finer == NULL) {
      Mc_ComputePartitionParams(ctrl, graph, wspace);
      Mc_KWayBalance(ctrl, graph, wspace, graph->ncon);
      Mc_KWayAdaptiveRefine(ctrl, graph, wspace, NGR_PASSES);
    }
  }
  else {
    /*******************************/
    /* Coarsen it and partition it */
    /*******************************/
    switch (ctrl->ps_relation) {
      case COUPLED:
        Mc_LocalMatch_HEM(ctrl, graph, wspace);
        break;
      case DISCOUPLED:
      default:
        Mc_GlobalMatch_Balance(ctrl, graph, wspace);
        break;
    }

    Adaptive_Partition(ctrl, graph->coarser, wspace);

    /********************************/
    /* project partition and refine */
    /********************************/
    Mc_ProjectPartition(ctrl, graph, wspace);
    Mc_ComputePartitionParams(ctrl, graph, wspace);

    if (graph->ncon > 1 && graph->level < 4) {
      Mc_ComputeParallelBalance(ctrl, graph, graph->where, lbvec);
      lbavg = savg(graph->ncon, lbvec);

      if (lbavg > ubavg + 0.025) {
        Mc_KWayBalance(ctrl, graph, wspace, graph->ncon);
      }
    }

    Mc_KWayAdaptiveRefine(ctrl, graph, wspace, NGR_PASSES);

    if (ctrl->dbglvl&DBG_PROGRESS) {
      Mc_ComputeParallelBalance(ctrl, graph, graph->where, lbvec);
      rprintf(ctrl, "nvtxs: %10d, cut: %8d, balance: ", graph->gnvtxs, graph->mincut);
      for (i=0; i<graph->ncon; i++) 
        rprintf(ctrl, "%.3f ", lbvec[i]);
      rprintf(ctrl, "\n");
    }
  }
}

