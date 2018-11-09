#! /bin/bash -l

#BSUB -q normal.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 4

echo "Running job"

hostname
mpirun "bin/exec"

echo "Job ended"

