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
TIMES=50;
THREAD=(1 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32)
algorithms=("ParallelSollinFAL" "FilterSollin")

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp(){
	$1 --runs 1 --min-threads $2 $3 ;
}

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp_algos(){
    for i in $(seq 1 $TIMES); do
        for thr in ${THREAD[@]}; do
            for algo in ${algorithms[@]}; do
                cmd_exp "$1" "$thr" "--USA-graph NW t --algorithm ${algo} --lsb-filename ${algo}_timing_USA_NW $2" ;
                cmd_exp "$1" "$thr" "--USA-graph CAL d --algorithm ${algo} --lsb-filename ${algo}_timing_USA_CAL $2" ;
            done
        done
    done
}

####################################################################################################


echo "Hostname=`hostname`" ;
echo "Number of available cores=`nproc --all`" ;
echo "Loading modules ..." ;

cd $PMST_PATH ;

echo "Running job" ;

cmd_exp_algos "$executable" ;

echo "Job ended" ;

