#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=24
#PBS -l walltime=00:30:00

cd $PBS_O_WORKDIR

module load openmpi-4.1.1+gnu-9.3.0

mpirun -np 1 ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./serial_time_jacoby.dat
# grep -v '^$' >./serial_time_jacoby.dat

mpirun -np 4 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./4_core_jacoby.dat

mpirun -np 8 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./8_core_jacoby.dat

mpirun -np 12 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./12_core_jacoby.dat

mpirun -np 4 --map-by socket ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./4_socket_jacoby.dat

mpirun -np 8 --map-by socket ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./8_socket_jacoby.dat

mpirun -np 12 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./12_socket_jacoby.dat

mpirun -np 12 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./12_node_jacoby.dat

mpirun -np 24 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./24_node_jacoby.dat

mpirun -np 48 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./48_node_jacoby.dat

exit
