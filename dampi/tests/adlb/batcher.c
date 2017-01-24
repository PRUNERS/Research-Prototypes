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
#include <string.h>

#include "adlb.h"

#define CMDLINE 1

int main(int argc, char *argv[])
{
  FILE *fp;
  int rc, i, done;
  char c, cmdbuffer[1024];

  int am_server, am_debug_server;
  int num_servers, use_debug_server, aprintf_flag;
  MPI_Comm app_comm;
  int my_world_rank, my_app_rank;

  int num_types = 1;
  int type_vect[2] = {CMDLINE};

  int work_prio, work_type, work_handle[ADLB_HANDLE_SIZE], work_len, answer_rank;
  int req_types[4];

  double start_time, end_time;

  rc = MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &my_world_rank );

  aprintf_flag = 0;		/* no output from adlb itself */
  num_servers = 1;		/* one server should be enough */
  use_debug_server = 0;		/* default: no debug server */
  rc = ADLB_Init(num_servers, use_debug_server, aprintf_flag, 1, type_vect,
		 &am_server, &am_debug_server, &app_comm);
  if ( !am_server && !am_debug_server ) /* application process */
    {
      MPI_Comm_rank( app_comm, &my_app_rank );
    }

  rc = MPI_Barrier( MPI_COMM_WORLD );
  start_time = MPI_Wtime();

  if ( am_server ) {
    ADLB_Server( 3000000, 0.0 );  
  }
  else if ( am_debug_server ) {
    ADLB_Debug_server( 300.0 );
  }
  else {                                 /* application process */
    if ( my_app_rank == 0 ) {  /* if master app, stuff the cmds into adlb */
      if (argc != 2) {
	printf("usage: %s <filename>\n", argv[0]);
	ADLB_Abort(-1);
      }
      else
	printf("command file is %s\n", argv[1]);

      fp = fopen(argv[1], "r");
      if (fp == NULL) {
	printf("could not open command file\n");
	ADLB_Abort(-1);
      } 

      while (fgets(cmdbuffer,1024,fp) != NULL) {
	cmdbuffer[strlen(cmdbuffer)] = '\0';
	printf("command = %s\n", cmdbuffer);
	if (cmdbuffer[0] != '#') {
	  /* put command into adlb here */
	  rc = ADLB_Put( cmdbuffer, strlen(cmdbuffer)+1, -1, -1, CMDLINE, 1 ); 
	  aprintf( 1, "put cmd, rc = %d\n", rc );
	}
      }
      printf("\nall commands submitted\n");
    }
    /* all application processes, including the master, execute this loop */
    /* but not an adlb server */
    done = 0;
    while ( !done ) {
      req_types[0] = -1;
      req_types[1] = req_types[2] = req_types[3] = -1;
      aprintf( 1, "Getting a command\n" );
      rc = ADLB_Reserve( req_types, &work_type, &work_prio, work_handle,
			 &work_len, &answer_rank); /* work handle is array, so no & */
      aprintf( 1, "rc from getting command = %d\n", rc );
      if ( rc == ADLB_DONE_BY_EXHAUSTION ) {
	aprintf( 1, "All jobs done\n" );
	break;
      }
      if ( rc == ADLB_NO_MORE_WORK ) {
	aprintf( 1, "No more work on reserve\n" );
	break;
      }
      else if (rc < 0) {
	aprintf( 1, "Reserve failed, rc = %d\n", rc );
	ADLB_Abort(-1);
      }
      else if ( work_type != CMDLINE) {
	aprintf( 1, "unexpected work type %d\n", work_type );
	ADLB_Abort( 99 );
      }
      else {			/* reserved good work */
	rc = ADLB_Get_reserved( cmdbuffer, work_handle );
	if (rc == ADLB_NO_MORE_WORK) {
	  aprintf( 1, "No more work on get_reserved\n" );
	  break;
	}
	else { 			/* got good work */
	  /* print command to be executed */
	  /* printf("executing command line :%s:\n", cmdbuffer); */
	  system( cmdbuffer );
	}
      }
    }
  }
  ADLB_Finalize();
  MPI_Finalize();

  return(0);
}
