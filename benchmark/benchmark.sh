#!/bin/bash
#
#SBATCH --mail-user=xxx@xxx.com
#SBATCH --mail-type=ALL
#SBATCH --job-name=parallel-image-processing
#SBATCH --output=/parallel-image-processing/benchmark/slurm/out/measurements.txt
#SBATCH --error=/parallel-image-processing/benchmark/slurm/err/measurements_err.txt
#SBATCH --chdir=/parallel-image-processing/build
#SBATCH --partition=fast
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=48
#SBATCH --mem-per-cpu=900
#SBATCH --exclusive
#SBATCH --time=4:00:00

PROGRAM_PATH="./Parallel-Image-Processing editor"
SIZES=("small" "mixture" "big")
THREADS=(2 4 6 8 12)

for size in "${SIZES[@]}"
do
    for i in {1..5}
    do
        output=$($PROGRAM_PATH $size s 1)
        echo "s,$size,$output"
    done
done

for size in "${SIZES[@]}"
do
    for threads in "${THREADS[@]}"
    do
        for i in {1..5}
        do
            output=$($PROGRAM_PATH $size parfiles $threads)
            echo "parfiles,$size,$threads,$output"
        done
    done
done

for size in "${SIZES[@]}"
do
    for threads in "${THREADS[@]}"
    do
        for i in {1..5}
        do
            output=$($PROGRAM_PATH $size parslices $threads)
            echo "parslices,$size,$threads,$output"
        done
    done
done

sleep 10

python ../benchmark/graph.py