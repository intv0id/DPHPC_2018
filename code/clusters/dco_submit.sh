#! /bin/bash -l

#SBATCH --job-name=MST_SUB
#SBATCH --output=job_%j.out
#SBATCH --nodes 4
#SBATCH --ntasks=16
#SBATCH --time=00:10:00 

echo "Running job"

hostname
srun mpirun "bin/exec"

echo "Job ended"

