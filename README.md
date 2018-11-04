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

### Install gcc 7.3.0 on EULER

```bash
cd /tmp

wget http://www.netgull.com/gcc/releases/gcc-7.3.0/gcc-7.3.0.tar.gz && \
tar xzf gcc-7.3.0.tar.gz && \
cd gcc-7.3.0 && \
./contrib/download_prerequisites && \
cd .. && \
mkdir objdir && \
cd objdir && \
$PWD/../gcc-7.3.0/configure --prefix=$HOME/gcc-7.3.0 --enable-languages=c,c++,fortran && \
make && \
make install && \
cd /tmp && \
rm -rf gcc-7.3.0.tar.gz gcc-7.3.0/

echo "# GCC 7.3 Compiler" >> ~/.bashrc
echo "export PATH=~/gcc-7.3.0/bin:$PATH" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=~/gcc-7.3.0/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=~/gcc-7.3.0/lib64:$LD_LIBRARY_PATH" >> ~/.bashrc

source ~/.bashrc
```

### Install liblsb on euler

``` bash
module load open_mpi/1.6.5 gcc/4.9.2
cd /tmp && wget https://spcl.inf.ethz.ch/Research/Performance/LibLSB/liblsb-0.2.2.tar.gz
tar xvf liblsb-0.2.2.tar.gz && rm liblsb-0.2.2.tar.gz
cd liblsb-0.2.2
./configure --prefix=`readlink -f ~/liblsb` && make && make install
cd .. && rm -r liblsb-0.2.2
```

### Compile & run `exec` 

``` bash
# import modules
module load open_mpi/1.6.5 gcc/4.9.2 cmake/3.5.2 boost/1.59.0
export LIBLSB_PATH=`readlink -f ~/liblsb/lib`

# Clone repo
cd ~
git clone https://github.com/intv0id/DPHPC_2018
cd DPHPC_2018/code

# Compile executable & submit job
cmake . && make
bsub < euler/submit.sh
```

**Note:** In order for this operation to work, the libraries must be statically compiled, or have to be downloaded in the cluster before using it.

## Dependencies
* MPI  
* LibLSB (https://spcl.inf.ethz.ch/Research/Performance/LibLSB/)
* Boost Graph Library (libboost-all-dev or something like that)
