#! /bin/bash -l

types=("ER" "PA");
declare -A algos=( [FAL]=ParallelSollinFAL [AL]=ParallelSollinAL [FS]=FilterSollin );
numbers=(50 10);

for short_algo in "${!algos[@]}"
do
    algo=${algos[$short_algo]}
    echo "$algo"
    for type in "${types[@]}"
    do
        for number in "${numbers[@]}"
	do
	    mv lsb.*$algo*$type*$number* ../lsb/$short_algo/$short_algo\_$type-$number
        done
    done
done
