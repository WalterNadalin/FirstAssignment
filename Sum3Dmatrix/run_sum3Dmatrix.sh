#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=00:30:00

cd $PBS_O_WORKDIR/

rm time_sum3Dmatrix.dat

module load openmpi-4.1.1+gnu-9.3.0
mpirun -np 24 ./sum3Dmatrix.x < input

exit
