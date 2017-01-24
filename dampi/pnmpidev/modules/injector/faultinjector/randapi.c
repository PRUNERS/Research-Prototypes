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

#include <stdlib.h>
#include <time.h>
#include "faultinjector.h"

/*
 * Init function 
 */
void 
initRand( void )
{
    time_t seed;
    seed = time(NULL);
    srand(seed);
    return;
}

/*
 * Generates a double random number from 0 to 1.0
 */
double 
getRandomDouble( void )
{
    double res;
    res = ((double)rand() / (double)RAND_MAX);
    return res;
}

/*
 * Generates an integer random number from 0 to (maxValue-1)
 */
int 
getRandomInteger(int maxValue) 
{
    int res;
    res = rand() % maxValue;
    return res;
}
