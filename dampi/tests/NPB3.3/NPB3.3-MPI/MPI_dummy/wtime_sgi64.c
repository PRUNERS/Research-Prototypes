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

#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/syssgi.h>
#include <sys/immu.h>
#include <errno.h>
#include <stdio.h>

/* The following works on SGI Power Challenge systems */

typedef unsigned long iotimer_t;

unsigned int cycleval;
volatile iotimer_t *iotimer_addr, base_counter;
double resolution;

/* address_t is an integer type big enough to hold an address */
typedef unsigned long address_t;



void timer_init() 
{
  
  int fd;
  char *virt_addr;
  address_t phys_addr, page_offset, pagemask, pagebase_addr;
  
  pagemask = getpagesize() - 1;
  errno = 0;
  phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cycleval);
  if (errno != 0) {
    perror("SGI_QUERY_CYCLECNTR");
    exit(1);
  }
  /* rel_addr = page offset of physical address */
  page_offset = phys_addr & pagemask;
  pagebase_addr = phys_addr - page_offset;
  fd = open("/dev/mmem", O_RDONLY);

  virt_addr = mmap(0, pagemask, PROT_READ, MAP_PRIVATE, fd, pagebase_addr);
  virt_addr = virt_addr + page_offset;
  iotimer_addr = (iotimer_t *)virt_addr;
  /* cycleval in picoseconds to this gives resolution in seconds */
  resolution = 1.0e-12*cycleval; 
  base_counter = *iotimer_addr;
}

void wtime_(double *time) 
{
  static int initialized = 0;
  volatile iotimer_t counter_value;
  if (!initialized) { 
    timer_init();
    initialized = 1;
  }
  counter_value = *iotimer_addr - base_counter;
  *time = (double)counter_value * resolution;
}


void wtime(double *time) 
{
  static int initialized = 0;
  volatile iotimer_t counter_value;
  if (!initialized) { 
    timer_init();
    initialized = 1;
  }
  counter_value = *iotimer_addr - base_counter;
  *time = (double)counter_value * resolution;
}


