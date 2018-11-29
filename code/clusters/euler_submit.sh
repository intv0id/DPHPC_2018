#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"



echo "Hostname=`hostname`"
echo "Number of available cores=`nproc --all`"
echo "Loading modules ..."
module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0

echo "Running job"

~/PMST/code/bin/exec --algorithm ParallelSollinEL --generate-graph 100 --generate-graph 1000 --generate-graph 10000 --lsb-filename SollinParallelEL --runs 10 --max-threads 32
~/PMST/code/bin/exec --algorithm ParallelSollinAL --generate-graph 100 --generate-graph 1000 --generate-graph 10000 --lsb-filename SollinParallelAL --runs 10 --max-threads 32
~/PMST/code/bin/exec --algorithm Sollin --generate-graph 100 --generate-graph 1000 --generate-graph 10000 --lsb-filename Sollin --runs 10 --max-threads 1
~/PMST/code/bin/exec --algorithm FilterKruskal --generate-graph 100 --generate-graph 1000 --generate-graph 10000 --lsb-filename FilterKruskal --runs 10 --max-threads 1
~/PMST/code/bin/exec --algorithm Kruskal --generate-graph 100 --generate-graph 1000 --generate-graph 10000 --lsb-filename Kruskal --runs 10 --max-threads 1

echo "Job ended"

