#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"


RUNS=1
MAX_THREAD=8

if [ -z ${PMST_PATH} ]; then 
echo "PMST_PATH not detected : switch to EULER mode"
PMST_PATH="`readlink -f ~`/PMST/code"
module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0
MAX_THREAD=32
RUNS=50
fi

echo "Hostname=`hostname`"
echo "Number of available cores=`nproc --all`"
echo "Loading modules ..."

echo "Running job"

cd $PMST_PATH
executable="${PMST_PATH}/bin/exec"

algorithms=("ParallelSollinEL ParallelSollinAL ParallelSollinFAL Sollin FilterKruskal Kruskal BoostKruskal FilterSollin")

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp(){
	$1 --runs ${RUNS} --max-threads ${MAX_THREAD} $2
}

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp_algos(){		
	cmd_exp "$1" " --Erdos-Renyi-graph 1000 --algorithm ${algorithms} --lsb-filename algo_timing $2" 
	cmd_exp "$1" " --Erdos-Renyi-graph 10000 --algorithm ${algorithms} --lsb-filename algo_timing $2" 
	cmd_exp "$1" " --Erdos-Renyi-graph 100000 --algorithm ${algorithms} --lsb-filename algo_timing $2" 
	cmd_exp "$1" " --USA-graph NY d --algorithm ${algorithms} --lsb-filename algo_timing $2" 
	cmd_exp "$1" " --USA-graph BAY t --algorithm ${algorithms} --lsb-filename algo_timing $2" 
}

cmd_exp_algos "$executable"

echo "Job ended"

