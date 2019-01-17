#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.120h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"



module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0 ;

PMST_PATH="`readlink -f ~`/PMST/code" ; 
executable="${PMST_PATH}/bin/exec" ;

# Variables
algorithms=("FilterKruskal" "Kruskal") ;
MAX_THREAD=32 ;
RUNS=3 ;
TIMES=1000 ;


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
    for i in $(seq 1 $TIMES); do
        for algo in ${algorithms[@]}; do
            cmd_exp "$1" " --USA-graph BAY d --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_USA_BAY $2" || true ;
            cmd_exp "$1" " --USA-graph NY d --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_USA_NY $2" || true ;
            cmd_exp "$1" " --PA-graph 50000 50 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_50k_50 $2" || true ;
            cmd_exp "$1" " --PA-graph 10000 1000 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_10k_1k $2" || true ;
            cmd_exp "$1" " --PA-graph 2000 20 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_2k_20 $2" || true ;
            cmd_exp "$1" " --PA-graph 1000 100 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_PA_1k_100 $2" || true ;
            cmd_exp "$1" " --Erdos-Renyi-graph 50000 100 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_ER_50k_100 $2" || true ;
            cmd_exp "$1" " --Erdos-Renyi-graph 10000 2000 --linear 2 --algorithm ${algo} --lsb-filename ${algo}_timing_ER_10k_2k $2" || true ;
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

