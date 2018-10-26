#! /bin/bash -l

#BSUB -q normal.4h
#BSUB -o %J.out
#BSUB -e %J.err
#BSUB -n 4

echo "Running job"

hostname
mpirun "bin/exec"

echo "Job ended"

