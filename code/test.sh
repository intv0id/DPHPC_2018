#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"

module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0 ;

PMST_PATH="`readlink -f ~`/PMST/code" ; 
executable="${PMST_PATH}/bin/exec" ;

# Variables
#MAX_THREAD=32 ;
TIMES=100;
#THREADS=(1 2 4 6 8 10 12 14 16 18)
THREADS=(1 2 4)
# THREAD=(1 32 16)
#algorithms=("FilterSollin" "ParallelSollinFAL" "ParallelSollinAL")
algorithms=("BoostBoruvka" "FilterSollin" "ParallelSollinFAL" "ParallelSollinAL"  "BoostMergeLocal" "BoostBoruvkaMixedMerge" "BoostBoruvkaThenMerge")
#algorithms="BoostMergeLocal BoostBoruvkaThenMerge BoostBoruvkaMixedMerge BoostBoruvka"
#algorithms="BoostBoruvka"
# algorithms="BoostBoruvka"

for i in $(seq 1 $TIMES); do
    for algo in ${algorithms[@]}; do
        for THREAD in ${THREADS[@]}; do
            echo "i: $i, algo: $algo, # threads: $THREAD"
            mpirun -n ${THREAD} ${executable} --runs 2 --min-threads ${THREAD} --PA-graph 2000 20 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_2k_20 >> out.out ;
            mpirun -n ${THREAD} ${executable} --runs 2 --min-threads ${THREAD} --PA-graph 1000 100 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_1k_100 >> out.out ;
        done
    done
done
