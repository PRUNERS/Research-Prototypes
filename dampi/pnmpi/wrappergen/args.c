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
 *   This code is an extension of the MPICH wrapper generator
 *   included in the MPICH 1.2.7p1 release. This code has been
 *   released with the following notice: 
 *
 *   Copyright (C) 1997 University of Chicago.
 *   See COPYRIGHT notice in the wrappergen directory.
 */

#include "args.h"
#include "expandingList.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG
#define DEBUG 0
#endif

int GetIntArg( argc, argv, switchName, val )
int *argc, *val;
char **argv, *switchName;
{
  int i, nremove;		/* nremove - number of arguments to remove */

  for (i=1; i<*argc; i++) {		  /* loop through all args */
    if (!strcmp( switchName, argv[i] )) { /* if this is the switch we want, */
      if (i+1<*argc) {			  /* make sure there is one more */
	if (sscanf( argv[i+1], "%d", val )) {
	  nremove = 2;			 /* got valid value */
	} else {
	  nremove = 1;			 /* didn't get valid value, but */
					 /* don't throw away the next arg. */
	}
      } else {
	nremove = 1;
      }
      for (i+=nremove; i<*argc; i++) {	 /* move everyone else down */
	argv[i-nremove]=argv[i];
      }
      (*argc)-=nremove;
      if (nremove==2) return 1;		 /* if we got a value, return */
      i--;
    }
  }
  return 0;
}



int GetDoubleArg( argc, argv, switchName, val )
int *argc;
double *val;
char **argv, *switchName;
{
  int i, nremove;		/* nremove - number of arguments to remove */

  for (i=1; i<*argc; i++) {		  /* loop through all args */
    if (!strcmp( switchName, argv[i] )) { /* if this is the switch we want, */
      if (i+1<*argc) {			  /* make sure there is one more */
	if (sscanf( argv[i+1], "%lf", val )) {
	  nremove = 2;			 /* got valid value */
	} else {
	  nremove = 1;			 /* didn't get valid value, but */
					 /* don't throw away the next arg. */
	}
      } else {
	nremove = 1;
      }
      for (i+=nremove; i<*argc; i++) {	 /* move everyone else down */
	argv[i-nremove]=argv[i];
      }
      (*argc)-=nremove;
      if (nremove==2) return 1;		 /* if we got a value, return */
      i--;
    }
  }
  return 0;
}



int GetStringArg( argc, argv, switchName, val )
int *argc;
char **argv, *switchName, **val;
{
  int i, nremove;		/* nremove - number of arguments to remove */

  for (i=1; i<*argc; i++) {		  /* loop through all args */
    if (!strcmp( switchName, argv[i] )) { /* if this is the switch we want, */
      if (i+1<*argc) {			  /* make sure there is one more */
	*val = argv[i+1];
	nremove = 2;
      } else {
	nremove = 1;
      }
      for (i+=nremove; i<*argc; i++) {	 /* move everyone else down */
	argv[i-nremove]=argv[i];
      }
      (*argc)-=nremove;
      if (nremove==2) return 1;		 /* if we got a value, return */
      i--;
    }
  }
  return 0;
}



int IsArgPresent( argc, argv, switchName )
int *argc;
char **argv, *switchName;
{
  int i, returnVal;

  returnVal = 0;
  for (i=1; i<*argc; i++) {		 /* loop through all args */
/*
    printf( "Comparing :%s: and :%s:\n", switchName, argv[i] );
*/
    if (!strcmp( switchName, argv[i] )) { /* if this is the switch we want, */
/*
      printf( "YUP!" );
*/
      for (i++; i<*argc; i++) {	 /* slide everything on down */
	argv[i-1]=argv[i];
      }
      (*argc)--;
      i--;
      returnVal = 1;
    }
  }
  return returnVal;
}




int GetArgAdjacentString( argc, argv, switchName, value )
int *argc;
char **argv, *switchName, **value;
{
  int argNum, str_len;
  xpandList listStr;
  char *readPtr, *theString;

  ListCreate( listStr, char, 10 );

  for (argNum=1; argNum<*argc; argNum++) {
    readPtr = strstr( argv[argNum], switchName );
    if (readPtr==argv[argNum]) {
      /* we want to find the switch at the beginning of an argument */
#if DEBUG
      fprintf( stderr, "Found %s in %s\n", switchName, argv[argNum] );
#endif
      readPtr = argv[argNum] + strlen( switchName );
      while (*readPtr) {
	ListAddItem( listStr, char, *readPtr ); /* add a character */
	readPtr++;
      }	/* done copying list */
      ListAddItem( listStr, char, '\0' ); /* terminate the string */

      for (argNum++; argNum < *argc; argNum++) {
	argv[argNum-1] = argv[argNum]; /* shift remaining arguments down */
      }
      (*argc)--;

      ListClose( listStr, char, theString, str_len );
#if DEBUG
      fprintf( stderr, "Returning string: %s\n", theString );
#endif
      *value = theString;
      return 1;
    } /* if strstr(... switch ) */
  } /* keep looking for the switch */
  return 0;			/* didn't even find the switch */
}




int GetIntListArg( argc, argv, switchName, intList, listLen )
int *argc;
char **argv, *switchName;
int **intList, *listLen;
{
  char *list, *token;
  xpandList tempIntList;
  int temp_int;
  
  ListCreate( tempIntList, int, 10 );

  if (!GetArgAdjacentString( argc, argv, switchName, &list )) {
    return 0;
  }
  token = strtok( list, "," );
  while (token) {
    if (sscanf( token, "%d", &temp_int ))
      ListAddItem( tempIntList, int, temp_int );
    token = strtok( (char*)0, "," );
  }

  ListClose( tempIntList, int, *intList, *listLen );
  free( list );
  return 1;
}

int GetStringListArg( argc, argv, switchName, strList, listLen )
int *argc, *listLen;
char **argv, *switchName, ***strList;
{
  char *list, *token, *str_dup;
  xpandList tempStrList;
  
  ListCreate( tempStrList, char *, 10 );
  if (!GetArgAdjacentString( argc, argv, switchName, &list )) {
    return 0;
  }
  token = strtok( list, "," );
  while (token) {
    str_dup = (char *) malloc( (strlen(token) + 1) * sizeof( char ) );
    strcpy( str_dup, token );
    ListAddItem( tempStrList, char *, str_dup );
#if DEBUG
    fprintf( stderr, "arg: get string list item :%s:\n", token );
#endif
    token = strtok( (char*)0, "," );
  }
  ListClose( tempStrList, char *, *strList, *listLen );
  free( list );
  return 1;
}

