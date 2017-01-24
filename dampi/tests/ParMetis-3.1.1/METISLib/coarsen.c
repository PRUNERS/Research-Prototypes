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
 * coarsen.c
 *
 * This file contains the driving routines for the coarsening process 
 *
 * Started 7/23/97
 * George
 *
 * $Id: coarsen.c,v 1.2 2003/07/31 16:23:29 karypis Exp $
 *
 */

#include <metis.h>


/*************************************************************************
* This function takes a graph and creates a sequence of coarser graphs
**************************************************************************/
GraphType *Coarsen2Way(CtrlType *ctrl, GraphType *graph)
{
  int clevel;
  GraphType *cgraph;

  IFSET(ctrl->dbglvl, DBG_TIME, starttimer(ctrl->CoarsenTmr));

  cgraph = graph;

  /* The following is ahack to allow the multiple bisections to go through with correct
     coarsening */
  if (ctrl->CType > 20) {
    clevel = 1;
    ctrl->CType -= 20;
  }
  else
    clevel = 0;

  do {
    IFSET(ctrl->dbglvl, DBG_COARSEN, printf("%6d %7d [%d] [%d %d]\n",
          cgraph->nvtxs, cgraph->nedges, ctrl->CoarsenTo, ctrl->maxvwgt, 
          (cgraph->vwgt ? idxsum(cgraph->nvtxs, cgraph->vwgt) : cgraph->nvtxs)));

    if (cgraph->adjwgt) {
      switch (ctrl->CType) {
        case MATCH_RM:
          Match_RM(ctrl, cgraph);
          break;
        case MATCH_HEM:
          if (clevel < 1 || cgraph->nedges == 0)
            Match_RM(ctrl, cgraph);
          else
            Match_HEM(ctrl, cgraph);
          break;
        case MATCH_SHEM:
          if (clevel < 1 || cgraph->nedges == 0)
            Match_RM(ctrl, cgraph);
          else
            Match_SHEM(ctrl, cgraph);
          break;
        case MATCH_SHEMKWAY:
          if (cgraph->nedges == 0)
            Match_RM(ctrl, cgraph);
          else
            Match_SHEM(ctrl, cgraph);
          break;
        default:
          errexit("Unknown CType: %d\n", ctrl->CType);
      }
    }
    else {
      Match_RM_NVW(ctrl, cgraph);
    }

    cgraph = cgraph->coarser;
    clevel++;

  } while (cgraph->nvtxs > ctrl->CoarsenTo && cgraph->nvtxs < COARSEN_FRACTION2*cgraph->finer->nvtxs && cgraph->nedges > cgraph->nvtxs/2); 

  IFSET(ctrl->dbglvl, DBG_COARSEN, printf("%6d %7d [%d] [%d %d]\n",
        cgraph->nvtxs, cgraph->nedges, ctrl->CoarsenTo, ctrl->maxvwgt, 
        (cgraph->vwgt ? idxsum(cgraph->nvtxs, cgraph->vwgt) : cgraph->nvtxs)));

  IFSET(ctrl->dbglvl, DBG_TIME, stoptimer(ctrl->CoarsenTmr));

  return cgraph;
}

