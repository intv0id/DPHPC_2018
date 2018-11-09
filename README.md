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

### Setup dependancies

Before doing anything:

``` bash
module load new gcc/5.2.0 open_mpi/1.6.5
```

After any `~/.bashrc`  changes:

``` bash
source ~/.bashrc
```

#### Install liblsb

``` bash
cd /tmp && wget https://spcl.inf.ethz.ch/Research/Performance/LibLSB/liblsb-0.2.2.tar.gz
tar xvf liblsb-0.2.2.tar.gz && rm liblsb-0.2.2.tar.gz
cd liblsb-0.2.2
./configure --prefix=`readlink -f ~/liblsb` && make && make install
cd .. && rm -r liblsb-0.2.2

echo "export LIBLSB_PATH=`readlink -f ~/liblsb`" >> ~/.bashrc
```

#### Install tbb

``` bash
cd /tmp
wget https://github.com/01org/tbb/releases/download/2019_U1/tbb2019_20181003oss_lin.tgz
tar xvf tbb2019_20181003oss_lin.tgz 
mkdir ~/tbb2019 && mv tbb2019_20181003oss/* ~/tbb2019/
echo "export TBB_PATH=`readlink -f ~/tbb2019`" >> ~/.bashrc
```

### Compile & run `exec` 

#### Required modules

``` bash
module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0 
```

#### Cloning / Updating files

``` bash
cd ~
git clone https://github.com/intv0id/PMST
```

**Update:**

``` bash
cd ~/PMST
git pull
```

#### Compile and submit to euler as a job

``` bash
cd ~/PMST/code

# Compile executable & submit job
cmake . && make
bsub < euler/submit.sh
```

**Note:** In order for this operation to work, the libraries must be statically compiled, or have to be downloaded in the cluster before using it.

## Dependencies
* MPI  
* LibLSB (https://spcl.inf.ethz.ch/Research/Performance/LibLSB/)
* Boost Graph Library (libboost-all-dev or something like that)
* TBB(libtbb-dev)
