#!/bin/bash

cd ~/mpi-benchmarks/src_c/

module purge
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
ldd IMB-MPI1 > ~/first_assignment/Benchmark/info.txt 

cd ~/first_assignment/Benchmark/ 

lstopo >> ./info.txt
ifconfig >> ./info.txt

mpirun -np 2 --map-by node --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>./bindings.txt |
grep -v ^# | 
grep -v '^$' >./benchmark.dat
# sed 's/.* \([0-9]\+\.[0-9]*\) .*/\1,/g' >./benchmark.txt

mpirun -np 2 --map-by socket --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 2 --map-by core --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 2 --mca pml ob1 --map-by core --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 1 --mca pml ucx --map-by core --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 2 --mca pml ob1 --map-by socket --mca btl tcp,self  --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29  2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 2 --mca pml ucx --map-by socket --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

mpirun -np 2 --mca pml ob1 --map-by node --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat

cd ~/mpi-benchmarks/src_c/

module purge
make clean
module load intel
make
ldd IMB-MPI1 >> ~/first_assignment/Benchmark/info.txt 

cd ~/first_assignment/Benchmark/

mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat
mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' >>./benchmark.dat
mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,25 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# |  
grep -v '^$' >>./benchmark.dat


