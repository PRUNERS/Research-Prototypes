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

#include "core.h"
#include <stdarg.h>

#ifdef DBGLEVEL
int _dbg_node = -2;
int _dbg_cur_level = DBGLEVEL;
int _dbg_cur_node  = -1;
#else
int _dbg_node = -2;
#endif

#ifndef NOSTATUS
int _status_node = 0;
#endif


/*-----------------------------------------------------------------*/
/* Not GNU */

#ifndef __GNUC__

void dbg_statusprint1(char *format,...);
void dbg_statusprintn(char *format,...);
void dbg_warning(char *format,...);
void dbg_assert(int cnd, char *format,...);
void dbg_debugprint1(char *format,...);
void dbg_debugprint2(char *format,...);
void dbg_debugprint3(char *format,...);
void dbg_debugprint4(char *format,...);

#ifdef NOSTATUS
void dbg_statusprint1(char *format,...) {}
void dbg_statusprintn(char *format,...) {}
#else

void dbg_statusprint1(char *format,...) 
{
  va_list va_alist;

  if (_status_node==0) 
    {
      if (_dbg_node>-2) 
	{
	  va_start(va_alist,format);
	  fprintf(stdout,"%5i: ",_dbg_node);
	  vfprintf(stdout,format,va_alist);  
	  fprintf(stdout,"\n");
	  va_end(va_alist);
	}
      else                                              
	{
	  va_start(va_alist,format);
	  vfprintf(stdout,format,va_alist);  
	  fprintf(stdout,"\n");
	  va_end(va_alist);
	}
    }
}

void dbg_statusprintn(char *format,...) 
{
  va_list va_alist;

  if (_dbg_node>-2) 
    {
      va_start(va_alist,format);
      fprintf(stdout,"%5i: ",_dbg_node);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
  else                                              
    {
      va_start(va_alist,format);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
}
#endif /* else of NOSTATUS */

#ifdef NOWARNINGS
void dbg_warning(char *format,...) {}
#else
void dbg_warning(char *format,...) 
{
  va_list va_alist;

  va_start(va_alist,format);
  fprintf(stdout,"WARNING: ");
  vfprintf(stdout,format,va_alist);  
  fprintf(stdout,"\n");
  va_end(va_alist);
}
#endif /* else of NOWARNINGS */

#ifdef NOASSERTS
void dbg_assert(int cnd, char *format,...) {}
#else
void dbg_assert(int cnd, char *format,...) 
{
  va_list va_alist;

  if (!(cnd))
    {
      va_start(va_alist,format);
      fprintf(stdout,"ASSERTION VIOLATED (%s/%i): ",__FILE__,__LINE__);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
      exit(1);
    }
}
#endif /* else of NOWARNINGS */

#ifndef DBGLEVEL
void dbg_debugprint1(char *format,...) {}
void dbg_debugprint2(char *format,...) {}
void dbg_debugprint3(char *format,...) {}
void dbg_debugprint4(char *format,...) {}
#else
void dbg_debugprint1(char *format,...)
{
  va_list va_alist;

  if (((DBGLEVEL1)&_dbg_cur_level) && 
      ((_dbg_cur_node==_dbg_node)||(_dbg_node<0)||(_dbg_cur_node<0)))
    {
      va_start(va_alist,format);
      fprintf(stdout,"L%04x ",format,_dbg_cur_level);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
}

void dbg_debugprint2(char *format,...)
{
  va_list va_alist;

  if (((DBGLEVEL2)&_dbg_cur_level) && 
      ((_dbg_cur_node==_dbg_node)||(_dbg_node<0)||(_dbg_cur_node<0)))
    {
      va_start(va_alist,format);
      fprintf(stdout,"L%04x ",format,_dbg_cur_level);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
}

void dbg_debugprint3(char *format,...)
{
  va_list va_alist;

  if (((DBGLEVEL3)&_dbg_cur_level) && 
      ((_dbg_cur_node==_dbg_node)||(_dbg_node<0)||(_dbg_cur_node<0)))
    {
      va_start(va_alist,format);
      fprintf(stdout,"L%04x ",format,_dbg_cur_level);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
}

void dbg_debugprint4(char *format,...)
{
  va_list va_alist;

  if (((DBGLEVEL4)&_dbg_cur_level) && 
      ((_dbg_cur_node==_dbg_node)||(_dbg_node<0)||(_dbg_cur_node<0)))
    {
      va_start(va_alist,format);
      fprintf(stdout,"L%04x ",format,_dbg_cur_level);
      vfprintf(stdout,format,va_alist);  
      fprintf(stdout,"\n");
      va_end(va_alist);
    }
}
#endif /* else of not def DBGLEVEL */

#endif /* __GNUC__*/
