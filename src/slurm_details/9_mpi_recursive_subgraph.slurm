#!/bin/bash  
#SBATCH -J 9_mpi_recursive_subgraph
#SBATCH -N 2
#SBATCH -n 8     

mpirun  -np 8 ./9_mpi_recursive_subgraph