#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=24
#PBS -l walltime=04:00:00

cd $PBS_O_WORKDIR/
rm time2.dat
rm time4.dat
rm time6.dat
rm time8.dat
rm info.txt
module load openmpi-4.1.1+gnu-9.3.0


for i in {2..48}

do
  	mpirun -np $i --map-by core ./ring2.x < iterations.txt
done

for i in {2..48}

do
  	mpirun -np $i --map-by core ./ring4.x < iterations.txt
done

for i in {2..48}

do
  	mpirun -np $i --map-by core ./ring6.x < iterations.txt
done


for i in {2..48}

do
  	mpirun -np $i --map-by core ./ring8.x < iterations.txt
done

exit
