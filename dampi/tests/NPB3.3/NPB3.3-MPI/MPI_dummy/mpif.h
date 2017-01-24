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

      integer mpi_comm_world
      parameter (mpi_comm_world = 0)

      integer mpi_max, mpi_min, mpi_sum
      parameter (mpi_max = 1, mpi_sum = 2, mpi_min = 3)

      integer mpi_byte, mpi_integer, mpi_real,
     >                  mpi_double_precision,  mpi_complex,
     >                  mpi_double_complex
      parameter (mpi_double_precision = 1,
     $           mpi_integer = 2, 
     $           mpi_byte = 3, 
     $           mpi_real= 4, 
     $           mpi_complex = 5,
     $           mpi_double_complex = 6)

      integer mpi_any_source
      parameter (mpi_any_source = -1)

      integer mpi_err_other
      parameter (mpi_err_other = -1)

      double precision mpi_wtime
      external mpi_wtime

      integer mpi_status_size
      parameter (mpi_status_size=3)
