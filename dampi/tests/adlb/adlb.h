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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#include "xq.h"

#include "mpi.h"

#define ADLB_SUCCESS                     (1)
#define ADLB_ERROR                      (-1)
#define ADLB_NO_MORE_WORK       (-999999999)
#define ADLB_DONE_BY_EXHAUSTION (-999999998)
#define ADLB_NO_CURRENT_WORK    (-999999997)
#define ADLB_PUT_REJECTED       (-999999996)
#define ADLB_LOWEST_PRIO        INT_MIN

/* for Info_get;  MUST match adlbf.h  */
#define ADLB_INFO_MALLOC_HWM               1
#define ADLB_INFO_AVG_TIME_ON_RQ           2
#define ADLB_INFO_NPUSHED_FROM_HERE        3
#define ADLB_INFO_NPUSHED_TO_HERE          4
#define ADLB_INFO_NREJECTED_PUTS           5
#define ADLB_INFO_LOOP_TOP_TIME            6
#define ADLB_INFO_MAX_QMSTAT_TRIP_TIME     7
#define ADLB_INFO_AVG_QMSTAT_TRIP_TIME     8
#define ADLB_INFO_NUM_QMS_EXCEED_INT       9
#define ADLB_INFO_NUM_RESERVES            10
#define ADLB_INFO_NUM_RESERVES_PUT_ON_RQ  11
#define ADLB_INFO_MAX_WQ_COUNT            12

#define ADLB_RESERVE_REQUEST_ANY    -1
#define ADLB_RESERVE_EOL            -1
#define ADLB_HANDLE_SIZE             5

int ADLBP_Init(int, int, int, int, int *, int *, int *, MPI_Comm *);
int ADLB_Init(int, int, int, int, int*, int *, int *, MPI_Comm *);

int ADLBP_Server(double hi_malloc, double periodic_logging_time);
int ADLB_Server(double hi_malloc, double periodic_logging_time);

int ADLBP_Debug_server(double timeout);
int ADLB_Debug_server(double timeout);

int ADLBP_Put(void *,int,int,int,int,int);
int ADLB_Put(void *,int,int,int,int,int);

int ADLBP_Reserve(int *, int *, int *, int *, int *, int *);
int ADLB_Reserve(int *, int *, int *, int *, int *, int *);

int ADLBP_Ireserve(int *, int *, int *, int *, int *, int *);
int ADLB_Ireserve(int *, int *, int *, int *, int *, int *);

int ADLBP_Get_reserved(void *, int *);
int ADLB_Get_reserved(void *, int *);

int ADLBP_Get_reserved_timed(void *, int *, double *);
int ADLB_Get_reserved_timed(void *, int *, double *);

int ADLBP_Begin_batch_put(void *, int);
int ADLBP_End_batch_put(void);

int ADLBP_Set_problem_done(void);
int ADLB_Set_problem_done(void);

int ADLBP_Set_no_more_work(void);  // deprecated
int ADLB_Set_no_more_work(void);

int ADLBP_Info_get(int, double *);
int ADLB_Info_get(int, double *);

int ADLBP_Info_num_work_units(int , int *, int *, int *);
int ADLB_Info_num_work_units(int , int *, int *, int *);

int ADLBP_Finalize(void);
int ADLB_Finalize(void);

int ADLBP_Abort(int);
int ADLB_Abort(int);

void dprintf(int flag, int linenum, char *fmt, ...);
#define aprintf(flag,...) dprintf(flag,__LINE__,__VA_ARGS__)
void *dmalloc(int,const char *,int);
#define amalloc(nbytes)   dmalloc(nbytes,__FUNCTION__,__LINE__)
void dfree(void *,int,const char *,int);
#define afree(ptr,nbytes) dfree(ptr,nbytes,__FUNCTION__,__LINE__)
