#! /bin/bash -l

#BSUB -q normal.4h
#BSUB -o %J.out
#BSUB -e %J.err
#BSUB -n 4

echo "Running job"

hostname

cd ~

tar xzf gcc-7.3.0.tar.gz && \
cd gcc-7.3.0 && \
./contrib/download_prerequisites && \
$PWD/configure --prefix=$HOME/gcc-7 --disable-multilib --enable-languages=c,c++ && \
make && \
make install && \
cd ~ && \
rm -rf gcc-7.3.0.tar.gz gcc-7.3.0/

echo "# GCC 7.3 Compiler" >> ~/.bashrc
echo "export PATH=~/gcc-7/bin:$PATH" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=~/gcc-7/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=~/gcc-7/lib64:$LD_LIBRARY_PATH" >> ~/.bashrc

source ~/.bashrc

echo "Job ended"
