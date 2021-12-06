#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:10:00

cd $PBS_O_WORKDIR/
rm time.dat
rm info.txt
module load openmpi-4.1.1+gnu-9.3.0

for i in {2..16}
do
  	mpirun -np $i ./ring.x
done

exit
