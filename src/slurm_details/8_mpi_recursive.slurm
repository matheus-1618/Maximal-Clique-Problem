#!/bin/bash  
#SBATCH -J 8_mpi_recursive
#SBATCH -N 2
#SBATCH -n 8     

mpirun  -np 8 ./8_mpi_recursive
