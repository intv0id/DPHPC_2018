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
algorithms=("FilterKruskal" "FilterSollin" "ParallelSollinFAL" "Kruskal") ;
MAX_THREAD=32 ;
RUNS=3 ;
TIMES=10 ;


# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp(){
	$1 --runs ${RUNS} --max-threads ${MAX_THREAD} $2 ;
}

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp_algos(){
    for algo in ${algorithms[@]}; do
	    for i in $(seq 1 $TIMES); do
            cmd_exp "$1" " --Erdos-Renyi-graph 100000 --algorithm ${algo} --lsb-filename ${algo}_ER_epv10k_100000 $2" ;
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

