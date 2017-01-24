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
#include <libconfig.h>

#define PARAM_ERROR(name) printf("Failed loading parameter: %s\n", name);

/* Enters to the configuration file and gets the faulty process.
  It prints its rank number to stdout */
int 
main(int argc, char *argv[], char *envp[])
{
    int id;
    struct config_t cfg;
	
    /* Get config file */
    char *filename;
    filename = getenv("FI_CONFIG_FILE");

    /* Initialize the configuration */
    config_init(&cfg);

    /* Load the file */
    if (!config_read_file(&cfg, filename)) {
	printf("Failed loading file: %s\n", filename);
	const char *text = config_error_text(&cfg);
	int line = config_error_line(&cfg);
	printf("%s\nProblem at line: %d\n", text, line);
	fflush(stdout);
	return 0;
    } else {
	config_setting_t *process =
	    config_lookup(&cfg, "experiment.faulty_process");

	if (!process) {
	    PARAM_ERROR("faulty_process");
	    fflush(stdout);
	    return 0;
	} else {
	    id = (int)config_setting_get_int(process);

	    if (id == -1) {
		printf("RANDOM\n");
	    } else {
		printf("%d\n", id);
	    }
	}
    }

    /* Free the configuration */
    config_destroy(&cfg);

    return 1;
}


