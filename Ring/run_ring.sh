#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=00:30:00

cd $PBS_O_WORKDIR/
rm time1.dat
rm time3.dat
rm time5.dat
rm time7.dat
rm info.txt
module load openmpi-4.1.1+gnu-9.3.0

for i in {2..24}

do
  	mpirun -np $i --map-by core ./ring1.x < iterations.txt
done

for i in {2..24}

do
  	mpirun -np $i --map-by core ./ring3.x < iterations.txt
done

for i in {2..24}

do
  	mpirun -np $i --map-by core ./ring5.x < iterations.txt
done


for i in {2..24}

do
  	mpirun -np $i --map-by core ./ring7.x < iterations.txt
done

exit
