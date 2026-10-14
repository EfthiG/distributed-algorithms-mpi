#!/bin/bash
set -x

# Runs make clean, removes old results, makes all, runs tests and makes graphs

# Run make clean, remove old results, make all
function clean_make {
    # make clean
    # if [ $? -ne 0 ]; then
    #     echo "make clean failed"
    #     exit 1
    # fi

    make all
    if [ $? -ne 0 ]; then
        echo "make all failed"
        exit 1
    fi
}


# Get system information (hostname, cpu model, cpu cores, os version, gcc version)
function get_system_info {
    {   echo "Hostname: $(hostname)"
        echo "CPU Model: $(lscpu | grep 'Model name' | awk -F: '{print $2}' | xargs)"
        echo "CPU Cores: $(nproc)"
        echo "OS Version: $(lsb_release -d | awk -F: '{print $2}' | xargs)"
        echo "GCC Version: $(gcc --version | head -n 1)"
    } > results/system_info.txt
}

NUMPROCS=(1 2 3 4)
NUM_RUNS=4
GENERATIONS=1000

# Run game of life tests
function run_game_of_life_tests {
    SIZES=(64 1024 2048)
    echo -e "Processes       Generations     Size            Time" > results/game_of_life.txt

    for num_proc in "${NUMPROCS[@]}"; do
        for size in "${SIZES[@]}"; do
            for run in $(seq 1 $NUM_RUNS); do
                mpirun -np $num_proc ./bin/game_of_life $GENERATIONS $size >> results/game_of_life.txt
            done
        done
    done
}

# Run matrix multiplication tests
function run_multiply_tests {
    SIZES=(60 240 2400 6000)

    echo -e "Processes       Size            Time" > results/multiply.txt
    for num_proc in "${NUMPROCS[@]}"; do
        for size in "${SIZES[@]}"; do
            for run in $(seq 1 $NUM_RUNS); do
                mpirun -np $num_proc ./bin/multiply $size >> results/multiply.txt
            done
        done
    done
}



function make_graphs {
    python3 graphs.py
}

clean_make
# get_system_info

# run_game_of_life_tests

run_multiply_tests

# make_graphs