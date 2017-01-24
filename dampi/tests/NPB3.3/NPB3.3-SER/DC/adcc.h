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
!-------------------------------------------------------------------------!
!				                                    	                  !
!		           N A S   G R I D   B E N C H M A R K S                  !
!									                                      !
!		                	C + +	V E R S I O N		                  !
!									                                      !
!			                       A D C C . H 		                      !
!									                                      !
!-------------------------------------------------------------------------!
!									                                      !
!    The the file contains comnstants definitions used for                !
!    building veiws.                                                      !
!									                                      !
!    Permission to use, copy, distribute and modify this software	      !
!    for any purpose with or without fee is hereby granted.		          !
!    We request, however, that all derived work reference the		      !
!    NAS Grid Benchmarks 3.0 or GridNPB3.0. This software is provided	  !
!    "as is" without expressed or implied warranty.			              !
!									                                      !
!    Information on GridNPB3.0, including the concept of		          !
!    the NAS Grid Benchmarks, the specifications, source code,  	      !
!    results and information on how to submit new results,		          !
!    is available at:							                          !
!									                                      !
!	  http://www.nas.nasa.gov/Software/NPB  			                  !
!									                                      !
!    Send comments or suggestions to  ngb@nas.nasa.gov  		          !
!    Send bug reports to	      ngb@nas.nasa.gov  		              !
!									                                      !
!	   E-mail:  ngb@nas.nasa.gov					                      !
!	   Fax:     (650) 604-3957					                          !
!									                                      !
!-------------------------------------------------------------------------!
! GridNPB3.0 C++ version						                          !
!	  Michael Frumkin, Leonid Shabanov				                      !
!-------------------------------------------------------------------------!
*/
#ifndef _ADCC_CONST_DEFS_H_
#define _ADCC_CONST_DEFS_H_

/*#define WINNT*/
#define UNIX

#define ADC_OK                        0
#define ADC_WRITE_FAILED              1
#define ADC_INTERNAL_ERROR            2
#define ADC_TREE_DESTROY_FAILURE      3
#define ADC_FILE_OPEN_FAILURE         4
#define ADC_MEMORY_ALLOCATION_FAILURE 5
#define ADC_FILE_DELETE_FAILURE       6
#define ADC_VERIFICATION_FAILED       7
#define ADC_SHMEMORY_FAILURE          8

#define SSA_BUFFER_SIZE     (1024*1024)
#define MAX_NUMBER_OF_TASKS         256

#define MAX_PAR_FILE_LINE_SIZE      512
#define MAX_FILE_FULL_PATH_SIZE     512
#define MAX_ADC_NAME_SIZE            32

#define DIM_FSZ                       4
#define MSR_FSZ                       8

#define MAX_NUM_OF_DIMS              20
#define MAX_NUM_OF_MEAS               4

#define MAX_NUM_OF_CHUNKS          1024      
#define MAX_PARAM_LINE_SIZE        1024

#define OUTPUT_BUFFER_SIZE (MAX_NUM_OF_DIMS + (MSR_FSZ/4)*MAX_NUM_OF_MEAS)
#define MAX_VIEW_REC_SIZE ((DIM_FSZ*MAX_NUM_OF_DIMS)+(MSR_FSZ*MAX_NUM_OF_MEAS))     
#define MAX_VIEW_ROW_SIZE_IN_INTS (MAX_NUM_OF_DIMS + 2*MAX_NUM_OF_MEAS)
#define MLB32  0x80000000

#ifdef WINNT
#define MLB    0x8000000000000000
#else
#define MLB 0x8000000000000000LL
#endif

#endif /*  _ADCC_CONST_DEFS_H_ */
