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
 * timer.c
 *
 * This file contain various timing routines
 *
 * Started 10/19/96
 * George
 *
 * $Id: timer.c,v 1.2 2003/07/21 17:18:54 karypis Exp $
 *
 */

#include <parmetislib.h>




/*************************************************************************
* This function initializes the various timers
**************************************************************************/
void InitTimers(CtrlType *ctrl)
{
  cleartimer(ctrl->TotalTmr); 
  cleartimer(ctrl->InitPartTmr);
  cleartimer(ctrl->MatchTmr); 
  cleartimer(ctrl->ContractTmr); 
  cleartimer(ctrl->CoarsenTmr); 
  cleartimer(ctrl->RefTmr);
  cleartimer(ctrl->SetupTmr); 
  cleartimer(ctrl->ProjectTmr); 
  cleartimer(ctrl->KWayInitTmr); 
  cleartimer(ctrl->KWayTmr);
  cleartimer(ctrl->MoveTmr);
  cleartimer(ctrl->RemapTmr);

  cleartimer(ctrl->AuxTmr1); 
  cleartimer(ctrl->AuxTmr2); 
  cleartimer(ctrl->AuxTmr3);
  cleartimer(ctrl->AuxTmr4); 
  cleartimer(ctrl->AuxTmr5); 
  cleartimer(ctrl->AuxTmr6);
}


/*************************************************************************
* This function prints timing information about KMETIS
**************************************************************************/
void PrintTimingInfo(CtrlType *ctrl)
{
/*  PrintTimer(ctrl, ctrl->CoarsenTmr,  " Coarsening"); */
  PrintTimer(ctrl, ctrl->SetupTmr,    "      Setup");
  PrintTimer(ctrl, ctrl->MatchTmr,    "   Matching");
  PrintTimer(ctrl, ctrl->ContractTmr, "Contraction");
  PrintTimer(ctrl, ctrl->InitPartTmr, "   InitPart");
/*  PrintTimer(ctrl, ctrl->RefTmr,      " Refinement"); */
  PrintTimer(ctrl, ctrl->ProjectTmr,  "    Project");
  PrintTimer(ctrl, ctrl->KWayInitTmr, " Initialize");
  PrintTimer(ctrl, ctrl->KWayTmr,     "      K-way");
  PrintTimer(ctrl, ctrl->MoveTmr,     "       Move");
  PrintTimer(ctrl, ctrl->RemapTmr,    "      Remap");
  PrintTimer(ctrl, ctrl->TotalTmr,    "      Total");
  PrintTimer(ctrl, ctrl->AuxTmr1,     "       Aux1");
  PrintTimer(ctrl, ctrl->AuxTmr2,     "       Aux2");
  PrintTimer(ctrl, ctrl->AuxTmr3,     "       Aux3");
  PrintTimer(ctrl, ctrl->AuxTmr4,     "       Aux4");
  PrintTimer(ctrl, ctrl->AuxTmr5,     "       Aux5");
  PrintTimer(ctrl, ctrl->AuxTmr6,     "       Aux6");
}


/*************************************************************************
* This function prints timer stat
**************************************************************************/
void PrintTimer(CtrlType *ctrl, timer tmr, char *msg)
{
  double sum, max, tsec;

  tsec = gettimer(tmr);
  MPI_Reduce((void *)&tsec, (void *)&sum, 1, MPI_DOUBLE, MPI_SUM, 0, ctrl->comm);

  tsec = gettimer(tmr);
  MPI_Reduce((void *)&tsec, (void *)&max, 1, MPI_DOUBLE, MPI_MAX, 0, ctrl->comm);

  if (ctrl->mype == 0 && sum != 0.0)
    printf("%s: Max: %7.3f, Sum: %7.3f, Balance: %7.3f\n", 
            msg, (float)max, (float)sum, (float)(max*ctrl->npes/sum));
}

