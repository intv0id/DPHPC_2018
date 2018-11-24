#! /bin/bash -l

#SBATCH --job-name=MST_SUB
#SBATCH --output=job_%j.out
#SBATCH --nodes 4
#SBATCH --ntasks=16
#SBATCH --time=00:10:00 

echo "Running job"

hostname
#srun mpirun 
/mnt/team10/PMST/code/bin/exec --algorithm ParallelSollinAL ParallelSollinEL --generate-graph 300 --generate-graph 3000 --generate-graph 30000 --lsb_filename dco_measures --runs 10 --max-threads 16

echo "Job ended"

