# Design of Parallel and High-Performances Computing 
**@ ETH Zürich - FALL 2018**

## Description

This project aims to experiment optimizations using parallel programming for the Minimum Spawning Tree problem.


## Authors

Thomas Cambier, 
Raphaël Dang-Nhu, 
Thibault Dardinier, 
Clément Trassoudaine

## Compiling
``` bash
cd code   
cmake -DBUILD\_TDEBUG=ON/OFF  
make  
./bin/exec  
```

## Writing Debug Code
``` C++
#ifdef DEBUG  
cout << x << endl;  
#endif 
```

## Cleaning folders
``` bash
cd code && ./clean.sh [cmake|measures]
```

## Run on EULER Cluster

**Doc:** (https://scicomp.ethz.ch/wiki/Getting\_started\_with\_clusters)

``` bash
module load open_mpi
cd ~
git clone https://github.com/intv0id/DPHPC_2018
cd DPHPC_2018/code
cmake . && make
bsub < euler/submit.sh
```

**Note:** In order for this operation to work, the libraries must be statically compiled, or have to be downloaded in the cluster before using it.

## Dependencies
* MPI  
* LibLSB (https://spcl.inf.ethz.ch/Research/Performance/LibLSB/)
* Boost Graph Library (libboost-all-dev or something like that)
