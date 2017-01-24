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

#ifndef _ADC_PARVIEW_TREE_DEF_H_
#define _ADC_PARVIEW_TREE_DEF_H_

#define MAX_TREE_HEIGHT	64
enum{BLACK,RED};

typedef struct treeNode{
  struct treeNode *left;
  struct treeNode *right;
  uint32 clr;
  int64 nodeMemPool[1];
} treeNode;

typedef struct RBTree{
  treeNode root;	
  treeNode * mp;
  uint32 count;       
  uint32 treeNodeSize;
  uint32 nodeDataSize;
  uint32 memoryLimit; 
  uint32 memaddr;
  uint32 memoryIsFull;
  uint32 freeNodeCounter;
  uint32 nNodesLimit;
  uint32 nd;
  uint32 nm;
  uint32   *drcts;
  treeNode **nodes;
  unsigned char * memPool;
} RBTree;

#define NEW_TREE_NODE(node_ptr,memPool,memaddr,treeNodeSize, \
 freeNodeCounter,memoryIsFull) \
 node_ptr=(struct treeNode*)(memPool+memaddr); \
 memaddr+=treeNodeSize; \
 (freeNodeCounter)--; \
 if( freeNodeCounter == 0 ) { \
     memoryIsFull = 1; \
 }

int32 TreeInsert(RBTree *tree, uint32 *attrs);

#endif /* _ADC_PARVIEW_TREE_DEF_H_ */
