# Design of Parallel and High-Performances Computing 
**@ ETH Zürich - FALL 2018**

[![Build Status](https://travis-ci.org/intv0id/PMST.svg?branch=master)](https://travis-ci.org/intv0id/PMST)

## Description

This project aims to experiment optimizations using parallel programming for the Minimum Spawning Tree problem.


## Authors

Thomas Cambier, 
Raphaël Dang-Nhu, 
Thibault Dardinier, 
Clément Trassoudaine


## Contributing, and using code

**See [this article](https://github.com/intv0id/PMST/wiki/Contributing)**

## Run on a HPC Cluster (EULER & DCO)

[**ETHZ documentation**](https://scicomp.ethz.ch/wiki/Getting\_started\_with\_clusters)

[**Install dependancies**](https://github.com/intv0id/PMST/wiki/Installing-libraries-locally)

[**Run jobs**](https://github.com/intv0id/PMST/wiki/Execute-a-job-on-EULER-&-DCO)


## Dependencies
* MPI  
* LibLSB (https://spcl.inf.ethz.ch/Research/Performance/LibLSB/)
* Boost Graph Library (libboost-all-dev or something like that)
* TBB(libtbb-dev)

## Benchmark
* Erdos-Renyi random graphs
* USA road network from the 9th DIMACS challenge (http://www.diag.uniroma1.it/challenge9/download.shtml)
* Stanford large networks dataset collection (https://snap.stanford.edu/data/)
