#!/bin/bash

python3 python/generate_graph.py $1 $2
g++ -Wall -O3 -g implementations/1_exaustive_recursive.cpp -o 1_exaustive_recursive
time ./1_exaustive_recursive
g++ -Wall -O3 -g implementations/2_exaustive_iterative.cpp -o 2_exaustive_iterative
time ./2_exaustive_iterative
g++ -Wall -O3 -g implementations/3_edges_heuristic.cpp -o 3_edges_heuristic
time ./3_edges_heuristic
g++ -Wall -O3 -g implementations/4_dynamic_programming.cpp -o 4_dynamic_programming
time ./4_dynamic_programming
g++ -Wall -O3 -g -fopenmp implementations/5_exaustive_recursive_parallel.cpp -o 5_exaustive_recursive_parallel
time ./5_exaustive_recursive_parallel
g++ -Wall -O3 -g -fopenmp implementations/6_exaustive_iterative_parallel.cpp -o 6_exaustive_iterative_parallel
time ./6_exaustive_iterative_parallel
g++ -Wall -O3 -g -fopenmp implementations/7_dynamic_programming_parallel.cpp -o 7_dynamic_programming_parallel
time ./7_dynamic_programming_parallel
time python3 python/verify_clique.py

# Delete compiled executables
rm  implementations/graph.txt 1_exaustive_recursive 2_exaustive_iterative 3_edges_heuristic 4_dynamic_programming 5_exaustive_recursive_parallel 6_exaustive_iterative_parallel 7_dynamic_programming_parallel
