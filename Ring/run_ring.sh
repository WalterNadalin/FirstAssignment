#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=04:00:00

cd $PBS_O_WORKDIR/

rm time.dat
rm info.txt

module load openmpi-4.1.1+gnu-9.3.0
mpirun -np 24 --map-by core ./ring.x < iterations.txt

exit
