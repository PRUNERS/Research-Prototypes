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
     >      , nv      ! size of rhs array
     >      , nr      ! size of residual array
     >      , nm2     ! size of communication buffer
     >      , maxlevel! maximum number of levels

      parameter( nm=2+2**lm, nv=(2+2**ndim1)*(2+2**ndim2)*(2+2**ndim3) )
      parameter( nm2=2*nm*nm, maxlevel=(lt_default+1) )
      parameter( nr = (8*(nv+nm**2+5*nm+14*lt_default-7*lm))/7 )
      integer maxprocs
      parameter( maxprocs = 131072 )  ! this is the upper proc limit that 
                                      ! the current "nr" parameter can handle
c---------------------------------------------------------------------
      integer nbr(3,-1:1,maxlevel), msg_type(3,-1:1)
      integer  msg_id(3,-1:1,2),nx(maxlevel),ny(maxlevel),nz(maxlevel)
      common /mg3/ nbr,msg_type,msg_id,nx,ny,nz

      character class
      common /ClassType/class

      integer debug_vec(0:7)
      common /my_debug/ debug_vec

      integer ir(maxlevel), m1(maxlevel), m2(maxlevel), m3(maxlevel)
      integer lt, lb
      common /fap/ ir,m1,m2,m3,lt,lb

      logical dead(maxlevel), give_ex(3,maxlevel), take_ex(3,maxlevel)
      common /comm_ex/ dead, give_ex, take_ex

c---------------------------------------------------------------------
c  Set at m=1024, can handle cases up to 1024^3 case
c---------------------------------------------------------------------
      integer m
c      parameter( m=1037 )
      parameter( m=nm+1 )

      double precision buff(nm2,4)
      common /buffer/ buff




