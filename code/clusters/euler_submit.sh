#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"


RUNS=5
MAX_THREAD=32
executable

echo "Hostname=`hostname`"
echo "Number of available cores=`nproc --all`"
echo "Loading modules ..."
#module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0

echo "Running job"

#path_exec="~/PMST/code/bin/exec"
path_exec="/home/raphael/Documents/4A/DPHPC/Repo/code/bin/exec"

list_algorithms=("ParallelSollinEL" "ParallelSollinAL" "Sollin" 'FilterKruskal' 'Kruskal')
erdos_graphs="--Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 "
usa_graphs="--USA-Graph NY d --USA-graph BAY t"

# Arguments:
# 1: path to executable
# 2: additional flags
cmd_exp(){
	$1 --runs ${RUNS} --max-threads ${MAX_THREAD} $2
}

# Arguments:
# 1: path to executable
# 2: list of graphs
# 3: additional flags
cmd_exp_algos(){
	for algo in ${list_algorithms[@]}; do
		cmd_exp "$1"  "${2} --algorithm ${algo} --lsb-filename ${algo}-ERG $3" 
	done;
}

cmd_exp_algos "$path_exec" "$erdos_graphs" 
cmd_exp_algos "$path_exec" "$usa_graphs" 

echo "Job ended"

