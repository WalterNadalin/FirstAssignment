#!/bin/bash

#PBS -q dssc_gpu
#PBS -l nodes=2:ppn=24
#PBS -l walltime=00:30:00

cd $PBS_O_WORKDIR

module load openmpi-4.1.1+gnu-9.3.0

mpirun -np 1 ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_serial_time_jacoby.dat
# grep -v '^$' >./serial_time_jacoby.dat

mpirun -np 4 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_4_core_jacoby.dat

mpirun -np 8 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_8_core_jacoby.dat

mpirun -np 12 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_12_core_jacoby.dat

mpirun -np 4 --map-by socket ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_4_socket_jacoby.dat

mpirun -np 8 --map-by socket ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_8_socket_jacoby.dat

mpirun -np 12 --map-by core ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_12_socket_jacoby.dat

mpirun -np 12 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_12_node_jacoby.dat

mpirun -np 24 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_24_node_jacoby.dat

mpirun -np 48 --map-by node ./jacoby3D.x < input.1200 |
grep '^  [0-9].' >./gpu_48_node_jacoby.dat

exit
