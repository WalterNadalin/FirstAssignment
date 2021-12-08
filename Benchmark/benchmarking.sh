#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=2
#PBS -l walltime=00:10:00

cd $PBS_O_WORKDIR

module load likwid
likwid-topology > info.txt
module unload likwid

cd ~/mpi-benchmarks/src_c/

module purge
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
ldd IMB-MPI1 >> ~/first_assignment/Benchmark/info.txt 

cd ~/first_assignment/Benchmark/ 

lstopo >> ./info.txt
ifconfig >> ./info.txt

mpirun -np 2 --map-by node --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>./bindings.txt |
grep -v ^# | 
grep -v '^$' >./benchmark1.dat
# sed 's/.* \([0-9]\+\.[0-9]*\) .*/\1,/g' >./benchmark.txt

mpirun -np 2 --map-by socket --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark2.dat

mpirun -np 2 --map-by core --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark3.dat

mpirun -np 2 --mca pml ob1 --map-by core --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark4.dat

mpirun -np 2 --mca pml ucx --map-by core --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark5.dat

mpirun -np 2 --mca pml ob1 --map-by socket --mca btl tcp,self  --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29  2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark6.dat

mpirun -np 2 --mca pml ucx --map-by socket --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark7.dat

mpirun -np 2 --mca pml ob1 --map-by node --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark8.dat

cd ~/mpi-benchmarks/src_c/

module purge
make clean
module load intel
make
ldd IMB-MPI1 >> ~/first_assignment/Benchmark/info.txt 

cd ~/first_assignment/Benchmark/

cat $PBS_NODEFILE | uniq > hosts.txt

mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark9.dat

mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >./benchmark10.dat

mpirun -n 2 -perhost 1 -machinefile hosts.txt \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# |  
grep -v '^$' >./benchmark11.dat

