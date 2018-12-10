#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"


RUNS=1
MAX_THREAD=4

if [ -z ${PMST_PATH} ]; then 
echo "PMST_PATH not detected : switch to EULER mode"
PMST_PATH ="~/PMST/code"
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

#list_algorithms=("ParallelSollinEL" "ParallelSollinAL" "Sollin" 'FilterKruskal' 'Kruskal')
list_algorithms=("ParallelSollinAL")
erdos_graphs="--Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 "
one_graph="--Erdos-Renyi-graph 400000"
one_graph_verif="--Erdos-Renyi-graph 1000"
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

cmd_exp_algos "$executable" "$one_graph" 
#cmd_exp_algos "$executable" "$one_graph_verif" "--verify" 
#cmd_exp_algos "$executable" "$usa_graphs" 

echo "Job ended"

