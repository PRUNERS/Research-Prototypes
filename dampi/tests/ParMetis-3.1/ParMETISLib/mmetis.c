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
 * mmetis.c
 *
 * This is the entry point of ParMETIS_V3_PartMeshKway
 *
 * Started 10/19/96
 * George
 *
 * $Id: mmetis.c,v 1.8 2003/07/25 04:01:04 karypis Exp $
 *
 */

#include <parmetislib.h>


/***********************************************************************************
* This function is the entry point of the parallel k-way multilevel mesh partitionioner. 
* This function assumes nothing about the mesh distribution.
* It is the general case.
************************************************************************************/
void ParMETIS_V3_PartMeshKway(idxtype *elmdist, idxtype *eptr, idxtype *eind, idxtype *elmwgt, 
                 int *wgtflag, int *numflag, int *ncon, int *ncommonnodes, int *nparts, 
		 float *tpwgts, float *ubvec, int *options, int *edgecut, idxtype *part, 
		 MPI_Comm *comm)
{
  printf("Entering ParMETIS_V3_PartKway in mmetis.c \n");
  int i, nvtxs, nedges, gnedges, npes, mype;
  idxtype *xadj, *adjncy;
  timer TotalTmr, Mesh2DualTmr, ParMETISTmr;
  CtrlType ctrl;

  /********************************/
  /* Try and take care bad inputs */
  /********************************/
  if (elmdist == NULL || eptr == NULL || eind == NULL || wgtflag == NULL || 
      numflag == NULL || ncon == NULL || ncommonnodes == NULL || nparts == NULL ||
      tpwgts == NULL || ubvec == NULL || options == NULL || edgecut == NULL || 
      part == NULL || comm == NULL) {
    printf("ERROR: One or more required parameters is NULL. Aborting.\n");
    abort();
  }
  if (((*wgtflag)&2) && elmwgt == NULL) {
    printf("ERROR: elmwgt == NULL when vertex weights were specified. Aborting.\n");
    abort();
  }

  
  SetUpCtrl(&ctrl, *nparts, (options[0] == 1 ? options[PMV3_OPTION_DBGLVL] : 0), *comm);
  npes = ctrl.npes;
  mype = ctrl.mype;

  cleartimer(TotalTmr);
  cleartimer(Mesh2DualTmr);
  cleartimer(ParMETISTmr);

  MPI_Barrier(ctrl.comm);
  starttimer(TotalTmr);
  starttimer(Mesh2DualTmr);

  ParMETIS_V3_Mesh2Dual(elmdist, eptr, eind, numflag, ncommonnodes, &xadj, &adjncy, &(ctrl.comm));

  if (ctrl.dbglvl&DBG_INFO) {
    nvtxs = elmdist[mype+1]-elmdist[mype];
    nedges = xadj[nvtxs] + (*numflag == 0 ? 0 : -1);
    rprintf(&ctrl, "Completed Dual Graph -- Nvtxs: %d, Nedges: %d \n", 
            elmdist[npes], GlobalSESum(&ctrl, nedges));
  }

  MPI_Barrier(ctrl.comm);
  stoptimer(Mesh2DualTmr);


  /***********************/
  /* Partition the graph */
  /***********************/
  starttimer(ParMETISTmr);

  ParMETIS_V3_PartKway(elmdist, xadj, adjncy, elmwgt, NULL, wgtflag, numflag, ncon, 
                       nparts, tpwgts, ubvec, options, edgecut, part, &(ctrl.comm));

  MPI_Barrier(ctrl.comm);
  stoptimer(ParMETISTmr);
  stoptimer(TotalTmr);

  IFSET(ctrl.dbglvl, DBG_TIME, PrintTimer(&ctrl, Mesh2DualTmr,	"   Mesh2Dual"));
  IFSET(ctrl.dbglvl, DBG_TIME, PrintTimer(&ctrl, ParMETISTmr,	"    ParMETIS"));
  IFSET(ctrl.dbglvl, DBG_TIME, PrintTimer(&ctrl, TotalTmr,	"       Total"));

  GKfree((void **)&xadj, (void **)&adjncy, LTERM);

  FreeCtrl(&ctrl);

  return;
}
