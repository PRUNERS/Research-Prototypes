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

c---------------------------------------------------------------------
c  Parameter lm (declared and set in "npbparams.h") is the log-base2 of 
c  the edge size max for the partition on a given node, so must be changed 
c  either to save space (if running a small case) or made bigger for larger 
c  cases, for example, 512^3. Thus lm=7 means that the largest dimension 
c  of a partition that can be solved on a node is 2^7 = 128. lm is set 
c  automatically in npbparams.h
c  Parameters ndim1, ndim2, ndim3 are the local problem dimensions. 
c---------------------------------------------------------------------

      include 'npbparams.h'

      integer nm      ! actual dimension including ghost cells for communications
c  ***  type of nv, nr and ir is set in npbparams.h
c     >      , nv      ! size of rhs array
c     >      , nr      ! size of residual array
     >      , maxlevel! maximum number of levels

      parameter( nm=2+2**lm, maxlevel=(lt_default+1) )
      parameter( nv=one*(2+2**ndim1)*(2+2**ndim2)*(2+2**ndim3) )
      parameter( nr = ((nv+nm**2+5*nm+7*lm+6)/7)*8 )
c---------------------------------------------------------------------
      integer  nx(maxlevel),ny(maxlevel),nz(maxlevel)
      common /mg3/ nx,ny,nz

      character class
      common /ClassType/class

      integer debug_vec(0:7)
      common /my_debug/ debug_vec

      integer m1(maxlevel), m2(maxlevel), m3(maxlevel)
      integer lt, lb
      common /fap/ ir(maxlevel),m1,m2,m3,lt,lb

c---------------------------------------------------------------------
c  Set at m=1024, can handle cases up to 1024^3 case
c---------------------------------------------------------------------
      integer m
c      parameter( m=1037 )
      parameter( m=nm+1 )

      logical timeron
      common /timers/ timeron
      integer T_init, T_bench, T_psinv, T_resid, T_rprj3, T_interp,
     >        T_norm2, T_mg3P, T_resid2, T_last
      parameter (T_init=1, T_bench=2, T_mg3P=3,
     >        T_psinv=4, T_resid=5, T_resid2=6, T_rprj3=7,
     >        T_interp=8, T_norm2=9, T_last=9)


