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
 * initmsection.c
 *
 * This file contains code that performs the k-way multisection
 *
 * Started 6/3/97
 * George
 *
 * $Id: initmsection.c,v 1.2 2003/07/21 17:18:49 karypis Exp $
 */

#include <parmetislib.h>


#define DEBUG_IPART_



/************************************************************************************/
/*! 
 The entry point of the algorithm that finds the separators of the coarsest graph.
 This algorithm first assembles the graph to all the processors, it then splits the
 processors into groups depending on the number of partitions for which we want to
 compute the separator. The processors of each group compute the separator of their
 corresponding graph and the smallest separator is selected.

 The parameter nparts on calling this routine indicates the number of desired
 partitions after the multisection (excluding the nodes that end up on the
 separator). The initial bisection is achieved when nparts==2 and upon entry
 graph->where[] = 0 for all vertices. Similarly, if nparts==4, it indicates that we
 have a graph that is already partitioned into two parts (as specified in
 graph->where) and we need to find the separator of each one of these parts.

 The final partitioning is encoded in the graph->where vector as follows. If nparts
 is the number of partitions, the left, right, and separator subpartitions of the
 original partition i will be labeled 2*i, 2*i+1, and nparts+2*i, respectively. Note
 that in the above expressions, i goes from [0...nparts/2]. As a result of this
 encoding, the left (i.e., 0th) partition of a node \c i on the separator will be
 given by where[i]%nparts. 

*/
/************************************************************************************/
void InitMultisection(CtrlType *ctrl, GraphType *graph, WorkSpaceType *wspace) 
{ 
  int i, lpecut[2], gpecut[2], mypart, moptions[10]; 
  idxtype *vtxdist, *gwhere = NULL, *part, *label; 
  GraphType *agraph; 
  int *sendcounts, *displs; 
  MPI_Comm newcomm, labelcomm;

  IFSET(ctrl->dbglvl, DBG_TIME, starttimer(ctrl->InitPartTmr));

  /* Assemble the graph and do the necessary pre-processing */
  agraph = AssembleMultisectedGraph(ctrl, graph, wspace);
  part = agraph->where;
  agraph->where = NULL;

  /* Split the processors into groups so that each one can do a bisection */
  mypart = ctrl->mype%(ctrl->nparts/2);
  MPI_Comm_split(ctrl->comm, mypart, 0, &newcomm);

  /* Each processor keeps the graph that it only needs and bisects it */
  agraph->ncon = 1; /* needed for Mc_KeepPart */
  Mc_KeepPart(agraph, wspace, part, mypart);
  label = agraph->label;  /* Save this because ipart may need it */
  agraph->label = NULL;

  /* Bisect the graph and construct the separator */
  switch (ctrl->ipart) {
    case ISEP_EDGE:
      moptions[0] = 1;
      moptions[1] = 3;
      moptions[2] = 1;
      moptions[3] = 1;
      moptions[4] = 0;
      moptions[7] = ctrl->mype;

      agraph->where = idxmalloc(agraph->nvtxs, "InitMultisection: agraph->where");

      METIS_EdgeComputeSeparator(&agraph->nvtxs, agraph->xadj, agraph->adjncy,
        agraph->vwgt, agraph->adjwgt, moptions, &agraph->mincut, agraph->where);
      break;
    case ISEP_NODE:
      moptions[0] = 1;
      moptions[1] = 3;
      moptions[2] = 1;
      moptions[3] = 2;
      moptions[4] = 0;
      moptions[7] = ctrl->mype;

      agraph->where = idxmalloc(agraph->nvtxs, "InitMultisection: agraph->where");

      METIS_NodeComputeSeparator(&agraph->nvtxs, agraph->xadj, agraph->adjncy, agraph->vwgt, 
            agraph->adjwgt, moptions, &agraph->mincut, agraph->where);
      break;
    default:
      errexit("Unknown ISEP type!\n");
  }

  for (i=0; i<agraph->nvtxs; i++) {
    ASSERT(ctrl, agraph->where[i]>=0 && agraph->where[i]<=2);
    if (agraph->where[i] == 2)
      agraph->where[i] = ctrl->nparts+2*mypart;
    else
      agraph->where[i] += 2*mypart;
  }

  /* Determine which PE got the minimum cut */
  lpecut[0] = agraph->mincut;
  MPI_Comm_rank(newcomm, lpecut+1);
  MPI_Allreduce(lpecut, gpecut, 1, MPI_2INT, MPI_MINLOC, newcomm);

  /* myprintf(ctrl, "Nvtxs: %d, Mincut: %d, GMincut: %d, %d\n", agraph->nvtxs, agraph->mincut, gpecut[0], gpecut[1]); */

  /* Send the best where to the root processor of this partition */
  if (lpecut[1] == gpecut[1] && gpecut[1] != 0) 
    MPI_Send((void *)agraph->where, agraph->nvtxs, IDX_DATATYPE, 0, 1, newcomm);
  if (lpecut[1] == 0 && gpecut[1] != 0)
    MPI_Recv((void *)agraph->where, agraph->nvtxs, IDX_DATATYPE, gpecut[1], 1, newcomm, &ctrl->status);

  /* Create a communicator that stores all the i-th processors of the newcomm */
  MPI_Comm_split(ctrl->comm, lpecut[1], 0, &labelcomm);

  /* Map the separator back to agraph. This is inefficient! */
  if (lpecut[1] == 0) {
    gwhere = idxsmalloc(graph->gnvtxs, 0, "InitMultisection: gwhere");
    for (i=0; i<agraph->nvtxs; i++)
      gwhere[label[i]] = agraph->where[i];
  }

  free(agraph->where);
  agraph->where = part;

  if (lpecut[1] == 0) {
    MPI_Reduce((void *)gwhere, (void *)agraph->where, graph->gnvtxs, IDX_DATATYPE, 
        MPI_SUM, 0, labelcomm);
    free(gwhere);
  }

  /* The minimum PE performs the Scatter */
  vtxdist = graph->vtxdist;
  ASSERT(ctrl, graph->where != NULL);
  GKfree((void **)&graph->where, LTERM);  /* Remove the propagated down where info */
  graph->where = idxmalloc(graph->nvtxs+graph->nrecv, "InitPartition: where");

  sendcounts = imalloc(ctrl->npes, "InitPartitionNew: sendcounts");
  displs     = imalloc(ctrl->npes, "InitPartitionNew: displs");

  for (i=0; i<ctrl->npes; i++) {
    sendcounts[i] = vtxdist[i+1]-vtxdist[i];
    displs[i]     = vtxdist[i];
  }

  MPI_Scatterv((void *)agraph->where, sendcounts, displs, IDX_DATATYPE, 
               (void *)graph->where, graph->nvtxs, IDX_DATATYPE, 0, ctrl->comm);

  GKfree((void **)&sendcounts, (void **)&displs, (void **)&label, LTERM);

  FreeGraph(agraph);

  MPI_Comm_free(&newcomm);
  MPI_Comm_free(&labelcomm);

  IFSET(ctrl->dbglvl, DBG_TIME, stoptimer(ctrl->InitPartTmr));

}




/*************************************************************************
* This function assembles the graph into a single processor
**************************************************************************/
GraphType *AssembleMultisectedGraph(CtrlType *ctrl, GraphType *graph, WorkSpaceType *wspace)
{
  int i, j, k, l, gnvtxs, nvtxs, gnedges, nedges, gsize;
  idxtype *xadj, *vwgt, *where, *adjncy, *adjwgt, *vtxdist, *imap;
  idxtype *axadj, *aadjncy, *aadjwgt, *avwgt, *awhere, *alabel;
  idxtype *mygraph, *ggraph;
  int *recvcounts, *displs, mysize;
  GraphType *agraph;

  gnvtxs  = graph->gnvtxs;
  nvtxs   = graph->nvtxs;
  nedges  = graph->xadj[nvtxs];
  xadj    = graph->xadj;
  vwgt    = graph->vwgt;
  where   = graph->where;
  adjncy  = graph->adjncy;
  adjwgt  = graph->adjwgt;
  vtxdist = graph->vtxdist;
  imap    = graph->imap;

  /* Determine the # of idxtype to receive from each processor */
  recvcounts = imalloc(ctrl->npes, "AssembleGraph: recvcounts");
  mysize = 3*nvtxs + 2*nedges;
  MPI_Allgather((void *)(&mysize), 1, MPI_INT, (void *)recvcounts, 1, MPI_INT, ctrl->comm);
  
  displs = imalloc(ctrl->npes+1, "AssembleGraph: displs");
  displs[0] = 0;
  for (i=1; i<ctrl->npes+1; i++) 
    displs[i] = displs[i-1] + recvcounts[i-1];

  /* Construct the one-array storage format of the assembled graph */
  mygraph = (mysize <= wspace->maxcore ? wspace->core : idxmalloc(mysize, "AssembleGraph: mygraph"));
  for (k=i=0; i<nvtxs; i++) {
    mygraph[k++] = xadj[i+1]-xadj[i];
    mygraph[k++] = vwgt[i];
    mygraph[k++] = where[i];
    for (j=xadj[i]; j<xadj[i+1]; j++) {
      mygraph[k++] = imap[adjncy[j]];
      mygraph[k++] = adjwgt[j];
    }
  }
  ASSERT(ctrl, mysize == k);

  /* Assemble the entire graph */
  gsize = displs[ctrl->npes];
  ggraph = (gsize <= wspace->maxcore-mysize ? wspace->core+mysize : idxmalloc(gsize, "AssembleGraph: ggraph"));
  MPI_Allgatherv((void *)mygraph, mysize, IDX_DATATYPE, (void *)ggraph, recvcounts, displs, IDX_DATATYPE, ctrl->comm);

  GKfree((void **)&recvcounts, (void **)&displs, LTERM);
  if (mysize > wspace->maxcore)
    free(mygraph);

  agraph = CreateGraph();
  agraph->nvtxs = gnvtxs;
  agraph->nedges = gnedges = (gsize-3*gnvtxs)/2;

  /* Allocate memory for the assembled graph */
  axadj = agraph->xadj = idxmalloc(gnvtxs+1, "AssembleGraph: axadj");
  avwgt = agraph->vwgt = idxmalloc(gnvtxs, "AssembleGraph: avwgt");
  awhere = agraph->where = idxmalloc(gnvtxs, "AssembleGraph: awhere");
  aadjncy = agraph->adjncy = idxmalloc(gnedges, "AssembleGraph: adjncy");
  aadjwgt = agraph->adjwgt = idxmalloc(gnedges, "AssembleGraph: adjwgt");
  alabel = agraph->label = idxmalloc(gnvtxs, "AssembleGraph: alabel");

  for (k=j=i=0; i<gnvtxs; i++) {
    axadj[i] = ggraph[k++];
    avwgt[i] = ggraph[k++];
    awhere[i] = ggraph[k++];
    for (l=0; l<axadj[i]; l++) {
      aadjncy[j] = ggraph[k++];
      aadjwgt[j] = ggraph[k++];
      j++;
    }
  }

  /* Now fix up the received graph */
  MAKECSR(i, gnvtxs, axadj);

  for (i=0; i<gnvtxs; i++)
    alabel[i] = i;

  if (gsize > wspace->maxcore-mysize)
    free(ggraph);

  return agraph;
}

