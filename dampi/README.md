<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#sec-1">1. License</a></li>
<li><a href="#sec-2">2. Introduction</a></li>
<li><a href="#sec-3">3. Configuration</a></li>
<li><a href="#sec-4">4. Installation</a></li>
<li><a href="#sec-5">5. Environment</a></li>
<li><a href="#sec-6">6. Usage</a></li>
</ul>
</div>
</div>


# License<a id="sec-1" name="sec-1"></a>

Copyright (c) 2015, Lawrence Livermore National Security, LLC.

Written by Anh Vo, Sriram Aananthakrishnan, Ganesh Gopalakrishnan,
Bronis R. de Supinski, Martin Schulz, and Greg Bronevetsky

Contact email: schulzm@llnl.gov, LLNL-CODE-647203

All rights reserved - please read information in "LICENSE".

# Introduction<a id="sec-2" name="sec-2"></a>

Distributed Analyzer for MPI (DAMPI) -  A scalable formal verification tool.

This project is Work in Progress.

# Configuration<a id="sec-3" name="sec-3"></a>

Enable/Disable modules in the configuration file "pnmpi/testpnmpi.conf".

# Installation<a id="sec-4" name="sec-4"></a>

    ./env-setup.sh
    make all
    sudo make install

# Environment<a id="sec-5" name="sec-5"></a>

    export PATH=/usr/local/dampi/bin:$PATH

Insert the export command in your "~/.bashrc" to set the environment
permanently.

# Usage<a id="sec-6" name="sec-6"></a>

Examples:

    cd tests

Example with no non-determinism:

    dmacc -I/usr/include/mpich2 monte.c -o monte
    dmaexec -N 2 monte
    scheduler -n 2 -k 4 -p monte

Example with non-determinism:

    dmacc -I/usr/include/mpich2 wildcard_Recv_three_matches.c -o wildcard_Recv_three_matches
    dmaexec -N 4 ./wildcard_Recv_three_matches
    scheduler -n 4 -k 1 -p ./wildcard_Recv_three_matches