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
 * macros.h
 *
 * This file contains macros used in multilevel
 *
 * Started 9/25/94
 * George
 *
 * $Id: macros.h,v 1.7 2003/07/21 19:11:40 karypis Exp $
 *
 */


/*************************************************************************
* The following macro returns a random number in the specified range
**************************************************************************/
#define RandomInRange(u) ((int)(1.0*(u)*rand()/(RAND_MAX+1.0)))

#define amax(a, b) ((a) >= (b) ? (a) : (b))
#define amin(a, b) ((a) >= (b) ? (b) : (a))

#define AND(a, b) ((a) < 0 ? ((-(a))&(b)) : ((a)&(b)))
#define OR(a, b) ((a) < 0 ? -((-(a))|(b)) : ((a)|(b)))
#define XOR(a, b) ((a) < 0 ? -((-(a))^(b)) : ((a)^(b)))

#define SWAP(a, b, tmp)  \
                 do {(tmp) = (a); (a) = (b); (b) = (tmp);} while(0) 

#define INC_DEC(a, b, val) \
                 do {(a) += (val); (b) -= (val);} while(0)


#define scopy(n, a, b) (float *)memcpy((void *)(b), (void *)(a), sizeof(float)*(n))
#define idxcopy(n, a, b) (idxtype *)memcpy((void *)(b), (void *)(a), sizeof(idxtype)*(n)) 

#define HASHFCT(key, size) ((key)%(size))


/*************************************************************************
* Timer macros
**************************************************************************/
#define cleartimer(tmr) (tmr = 0.0)
#define starttimer(tmr) (tmr -= seconds())
#define stoptimer(tmr) (tmr += seconds())
#define gettimer(tmr) (tmr)


/*************************************************************************
* This macro is used to handle dbglvl
**************************************************************************/
#define IFSET(a, flag, cmd) if ((a)&(flag)) (cmd);

/*************************************************************************
* These macros are used for debuging memory leaks
**************************************************************************/
#ifdef DMALLOC
#define imalloc(n, msg) (malloc(sizeof(int)*(n)))
#define fmalloc(n, msg) (malloc(sizeof(float)*(n)))
#define idxmalloc(n, msg) (malloc(sizeof(idxtype)*(n)))
#define ismalloc(n, val, msg) (iset((n), (val), malloc(sizeof(int)*(n))))
#define idxsmalloc(n, val, msg) (idxset((n), (val), malloc(sizeof(idxtype)*(n))))
#define GKmalloc(a, b) (malloc((a)))
#endif

#ifdef DMALLOC
#   define MALLOC_CHECK(ptr);
/*
#   define MALLOC_CHECK(ptr)                                          \
    if (malloc_verify((ptr)) == DMALLOC_VERIFY_ERROR) {  \
        printf("***MALLOC_CHECK failed on line %d of file %s: " #ptr "\n", \
              __LINE__, __FILE__);                               \
        abort();                                                \
    }
*/
#else
#   define MALLOC_CHECK(ptr) ;
#endif 



/*************************************************************************
* This macro converts a length array in a CSR one
**************************************************************************/
#define MAKECSR(i, n, a) \
   do { \
     for (i=1; i<n; i++) a[i] += a[i-1]; \
     for (i=n; i>0; i--) a[i] = a[i-1]; \
     a[0] = 0; \
   } while(0) 


/*************************************************************************
* These macros insert and remove nodes from the boundary list
**************************************************************************/
#define BNDInsert(nbnd, bndind, bndptr, vtx) \
   do { \
     ASSERT(bndptr[vtx] == -1); \
     bndind[nbnd] = vtx; \
     bndptr[vtx] = nbnd++;\
   } while(0) 

#define BNDDelete(nbnd, bndind, bndptr, vtx) \
   do { \
     ASSERT(bndptr[vtx] != -1); \
     bndind[bndptr[vtx]] = bndind[--nbnd]; \
     bndptr[bndind[nbnd]] = bndptr[vtx]; \
     bndptr[vtx] = -1; \
   } while(0) 



/*************************************************************************
* These are debugging macros
**************************************************************************/
#ifdef DEBUG
#   define ASSERT(expr)                                          \
    if (!(expr)) {                                               \
        printf("***ASSERTION failed on line %d of file %s: " #expr "\n", \
              __LINE__, __FILE__);                               \
        abort();                                                \
    }
#else
#   define ASSERT(expr) ;
#endif 

#ifdef DEBUG
#   define ASSERTP(expr, msg)                                          \
    if (!(expr)) {                                               \
        printf("***ASSERTION failed on line %d of file %s: " #expr "\n", \
              __LINE__, __FILE__);                               \
        printf msg ; \
        abort();                                                \
    }
#else
#   define ASSERTP(expr, msg) ;
#endif 
