#!/bin/bash

#PBS -q dssc_gpu
#PBS -l nodes=2:ppn=24
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
grep -v '^$' |
sed '1d' >./benchmark1.dat

mpirun -np 2 --map-by socket --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark2.dat

mpirun -np 2 --map-by core --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark3.dat

mpirun -np 2 --mca pml ob1 --map-by core --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark4.dat

mpirun -np 2 --mca pml ucx --map-by core --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark5.dat

mpirun -np 2 --mca pml ob1 --map-by socket --mca btl tcp,self  --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29  2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark6.dat

mpirun -np 2 --mca pml ucx --map-by socket --mca btl vader --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark7.dat

mpirun -np 2 --mca pml ob1 --map-by node --mca btl tcp,self --report-bindings \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark8.dat

for i in {1..9}
do

  mpirun -np 2 --map-by node \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 |
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark1.dat

  mpirun -np 2 --map-by socket \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark2.dat

  mpirun -np 2 --map-by core \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark3.dat

  mpirun -np 2 --mca pml ob1 --map-by core --mca btl tcp,self \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark4.dat

  mpirun -np 2 --mca pml ucx --map-by core --mca btl vader \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark5.dat

  mpirun -np 2 --mca pml ob1 --map-by socket --mca btl tcp,self \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark6.dat

  mpirun -np 2 --mca pml ucx --map-by socket --mca btl vader \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark7.dat

  mpirun -np 2 --mca pml ob1 --map-by node --mca btl tcp,self \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark8.dat

done

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
grep -v '^$' |
sed '1d' >./benchmark9.dat

mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# | 
grep -v '^$' |
sed '1d' >./benchmark10.dat

mpirun -n 2 -perhost 1 -machinefile hosts.txt \
~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>>./bindings.txt | 
grep -v ^# |  
grep -v '^$' |
sed '1d' >./benchmark11.dat

for i in {1..10}
do

  mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark9.dat

  mpirun -n 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# | 
  grep -v '^$' |
  sed '1d' >>./benchmark10.dat

  mpirun -n 2 -perhost 1 -machinefile hosts.txt \
  ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 | 
  grep -v ^# |  
  grep -v '^$' |
  sed '1d' >>./benchmark11.dat

done

exit
