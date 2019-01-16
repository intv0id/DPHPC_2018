#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.24h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"

module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0 ;

PMST_PATH="`readlink -f ~`/PMST/code" ; 
executable="${PMST_PATH}/bin/exec" ;

# Variables
#algorithms=("FilterKruskal" "Kruskal" "BoostKruskal" "BoostPrim") ;
#MAX_THREAD=32 ;
TIMES=100;
THREAD=8;
# THREAD=(1 32 16)
#algorithms=("FilterSollin" "ParallelSollinFAL" "ParallelSollinAL")
#algorithms=("BoostBoruvka" "BoostMergeLocal" "BoostBoruvkaMixedMerge" "BoostBoruvkaThenMerge")
algorithms="BoostMergeLocal BoostBoruvkaThenMerge BoostBoruvkaMixedMerge BoostBoruvka"
#algorithms=("ParallelSollinFAL" "FilterSollin")


# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp(){
	# $1 --runs 10 --min-threads $2 $3 ;
	# $1 --runs 5 --min-threads $2 $3 ;
    echo $3
	mpiexec -n $2 $1 --runs 3 --min-threads $2 $3 ;
}

mpiexec -n ${THREAD} ${executable} --runs 2 --min-threads ${THREAD} --PA-graph 1000 100 --algorithm ${algorithms} --lsb-filename ${algo}_timing_PA_1k ;
