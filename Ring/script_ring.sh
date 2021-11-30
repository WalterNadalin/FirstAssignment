#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=24
#PBS -l walltime=0:10:00

cd $PBS_O_WORKDIR/
rm time.dat
module load openmpi-4.1.1+gnu-9.3.0

for i in {2..48}
do
  	mpirun -np $i ./ring.x
done

exit
