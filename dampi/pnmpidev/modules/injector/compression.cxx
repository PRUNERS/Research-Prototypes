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

/*========================================================*/
/* Compression Type */

#define COMPRESSION_TYPE MPI_DOUBLE
typedef double compression_type;


/*========================================================*/
/* Compression Prototypes */

#define NUMCOMPRESSION 2
#define NAMELEN        20

void compression_1(int packet, int total, int *compressed,
		   PNMPIMOD_Datatype_Parameters_t *ref);
void compression_2(int packet, int total, int *compressed,
		   PNMPIMOD_Datatype_Parameters_t *ref);


/*========================================================*/
/* Other types */

typedef char name_t[NAMELEN];
typedef void (*compression_fct_t)(int,int,int*,PNMPIMOD_Datatype_Parameters_t*);

/* Arguments for the compression function 

   1.) Packet size in bits
   2.) Number of elements in total message
   2.) Return number of bits of compressed data
   3.) iterator for datatype
*/

/*========================================================*/
/* Initialize the table with all compression functions */
/*
   In: pointer to table with all functions
       pointer to table with all names
  Out: number of compression functions 
*/

int create_compression_table(compression_fct_t **fct_table,
			     name_t **name_table)
{
  *fct_table=(compression_fct_t*) 
    malloc(NUMCOMPRESSION*sizeof(compression_fct_t*));
  *name_table=(name_t*) 
    malloc(NUMCOMPRESSION*sizeof(name_t));
  
  if ((*fct_table==NULL) || (*name_table==NULL))
    {
      fprintf(stderr,"Can't allocate compression and name tables\n");
      exit(1);
    }

  (*fct_table)[0]=(compression_fct_t) compression_1;
  (*fct_table)[1]=(compression_fct_t) compression_2;

  strcpy((*name_table)[0],"compr1");
  strcpy((*name_table)[1],"compr2");
	   
  return NUMCOMPRESSION;
}


/*========================================================*/
/* Compression function 1 */

void compression_1(int packet, int total, int *compressed,
		   PNMPIMOD_Datatype_Parameters_t *ref)
{
  /* example sum up all elements of the message */

  int t, l, s, done, i;
  char* b;
  compression_type sum; 

  sum=0.0;

  do
    {
      PNMPIMOD_Datatype_getItem(&ref,&b,&t,&l,&s,&done)
#ifdef USE_FUNCTIONS
	;
#endif
      /* assume that t is of compression_type and s=sizeof(t) */

      for (i=0; i<l; i++)
	{
	  sum+=((compression_type*)b)[i];
	}
    }
  while (!done);

  *compressed=(sizeof(compression_type)*total)/2;
}


/*========================================================*/
/* Compression function 1 */

void compression_2(int packet, int total, int *compressed,
		   PNMPIMOD_Datatype_Parameters_t *ref)
{
  /* add iterations */

  *compressed=(sizeof(compression_type)*total)/4;
}


/*========================================================*/
/* The end. */

