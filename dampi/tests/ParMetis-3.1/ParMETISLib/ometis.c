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
 * ometis.c
 *
 * This is the entry point of parallel ordering
 *
 * Started 10/19/96
 * George
 *
 * $Id: ometis.c,v 1.4 2003/07/25 04:01:04 karypis Exp $
 *
 */

#include <parmetislib.h>




/***********************************************************************************
* This function is the entry point of the parallel ordering algorithm.
* This function assumes that the graph is already nice partitioned among the 
* processors and then proceeds to perform recursive bisection.
************************************************************************************/
void ParMETIS_V3_NodeND(idxtype *vtxdist, idxtype *xadj, idxtype *adjncy, int *numflag,
              int *options, idxtype *order, idxtype *sizes, MPI_Comm *comm)
{
  int i, j;
  int ltvwgts[MAXNCON];
  int nparts, npes, mype, wgtflag = 0, seed = GLOBAL_SEED;
  CtrlType ctrl;
  WorkSpaceType wspace;
  GraphType *graph, *mgraph;
  idxtype *morder;
  int minnvtxs;

  MPI_Comm_size(*comm, &npes);
  MPI_Comm_rank(*comm, &mype);
  nparts = npes;

  if (!ispow2(npes)) {
    if (mype == 0)
      printf("Error: The number of processors must be a power of 2!\n");
    return;
  }

  if (vtxdist[npes] < (int)((float)(npes*npes)*1.2)) {
    if (mype == 0)
      printf("Error: Too many processors for this many vertices.\n");
    return;
  }

  minnvtxs = vtxdist[1]-vtxdist[0];
  for (i=0; i<npes; i++)
    minnvtxs = (minnvtxs < vtxdist[i+1]-vtxdist[i]) ? minnvtxs : vtxdist[i+1]-vtxdist[i];

  if (minnvtxs < (int)((float)npes*1.1)) {
    if (mype == 0)
      printf("Error: vertices are not distributed equally.\n");
    return;
  }
 

  if (*numflag == 1) 
    ChangeNumbering(vtxdist, xadj, adjncy, order, npes, mype, 1);

  SetUpCtrl(&ctrl, nparts, options[PMV3_OPTION_DBGLVL], *comm);
  ctrl.CoarsenTo = amin(vtxdist[npes]+1, 25*npes);

  ctrl.CoarsenTo = amin(vtxdist[npes]+1, 25*amax(npes, nparts));
  ctrl.seed = mype;
  ctrl.sync = seed;
  ctrl.partType = STATIC_PARTITION;
  ctrl.ps_relation = -1;
  ctrl.tpwgts = fsmalloc(nparts, 1.0/(float)(nparts), "tpwgts");
  ctrl.ubvec[0] = 1.03;

  graph = Moc_SetUpGraph(&ctrl, 1, vtxdist, xadj, NULL, adjncy, NULL, &wgtflag);

  PreAllocateMemory(&ctrl, graph, &wspace);

  /*=======================================================
   * Compute the initial k-way partitioning 
   =======================================================*/
  IFSET(ctrl.dbglvl, DBG_TIME, InitTimers(&ctrl));
  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, starttimer(ctrl.TotalTmr));

  Moc_Global_Partition(&ctrl, graph, &wspace);

  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, stoptimer(ctrl.TotalTmr));
  IFSET(ctrl.dbglvl, DBG_TIME, PrintTimingInfo(&ctrl));

  /*=======================================================
   * Move the graph according to the partitioning
   =======================================================*/
  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, starttimer(ctrl.MoveTmr));

  MALLOC_CHECK(NULL);
  graph->ncon = 1;
  mgraph = Moc_MoveGraph(&ctrl, graph, &wspace);
  MALLOC_CHECK(NULL);

  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, stoptimer(ctrl.MoveTmr));

  /*=======================================================
   * Now compute an ordering of the moved graph
   =======================================================*/
  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, starttimer(ctrl.TotalTmr));

  FreeWSpace(&wspace);
  PreAllocateMemory(&ctrl, mgraph, &wspace);

  ctrl.ipart = ISEP_NODE;
  ctrl.CoarsenTo = amin(vtxdist[npes]+1, amax(20*npes, 1000));

  /* compute tvwgts */
  for (j=0; j<mgraph->ncon; j++)
    ltvwgts[j] = 0;

  for (i=0; i<mgraph->nvtxs; i++)
    for (j=0; j<mgraph->ncon; j++)
      ltvwgts[j] += mgraph->vwgt[i*mgraph->ncon+j];

  for (j=0; j<mgraph->ncon; j++)
    ctrl.tvwgts[j] = GlobalSESum(&ctrl, ltvwgts[j]);

  mgraph->nvwgt = fmalloc(mgraph->nvtxs*mgraph->ncon, "mgraph->nvwgt");
  for (i=0; i<mgraph->nvtxs; i++)
    for (j=0; j<mgraph->ncon; j++)
      mgraph->nvwgt[i*mgraph->ncon+j] = (float)(mgraph->vwgt[i*mgraph->ncon+j]) / (float)(ctrl.tvwgts[j]);


  morder = idxmalloc(mgraph->nvtxs, "PAROMETIS: morder");
  MultilevelOrder(&ctrl, mgraph, morder, sizes, &wspace);

  MALLOC_CHECK(NULL);

  /* Invert the ordering back to the original graph */
  ProjectInfoBack(&ctrl, graph, order, morder, &wspace);

  MALLOC_CHECK(NULL);

  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));
  IFSET(ctrl.dbglvl, DBG_TIME, stoptimer(ctrl.TotalTmr));
  IFSET(ctrl.dbglvl, DBG_TIME, PrintTimingInfo(&ctrl));
  IFSET(ctrl.dbglvl, DBG_TIME, MPI_Barrier(ctrl.gcomm));

  free(ctrl.tpwgts);
  free(morder);
  FreeGraph(mgraph);
  FreeInitialGraphAndRemap(graph, 0);
  FreeWSpace(&wspace);
  FreeCtrl(&ctrl);

  if (*numflag == 1) 
    ChangeNumbering(vtxdist, xadj, adjncy, order, npes, mype, 0);

  MALLOC_CHECK(NULL);
}


/***********************************************************************************
* This function is the entry point of the parallel ordering algorithm.
* This function assumes that the graph is already nice partitioned among the 
* processors and then proceeds to perform recursive bisection.
************************************************************************************/
void PAROMETIS(idxtype *vtxdist, idxtype *xadj, idxtype *vwgt, idxtype *adjncy, idxtype *adjwgt, 
                idxtype *order, idxtype *sizes, int *options, MPI_Comm comm)
{
  int numflag, newoptions[5];

  newoptions[0] = 1;
  newoptions[PMV3_OPTION_DBGLVL] = options[4];
  newoptions[PMV3_OPTION_SEED] = GLOBAL_SEED;

  numflag = options[3];

  ParMETIS_V3_NodeND(vtxdist, xadj, adjncy, &numflag, newoptions, order, sizes, &comm);

  options[0] = -1;

}

