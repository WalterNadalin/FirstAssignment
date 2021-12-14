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

echo -e "#header_line 1: mpirun -np 2 --map-by node ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29\n#header_line 2:" > benchmark1.dat
cat $PBS_NODEFILE | uniq >> benchmark1.dat
echo -e "#header_line 3:\n#header: #bytes #repetitions t[usec] Mbytes/sec" >> benchmark1.dat
 
mpirun -np 2 --map-by node --report-bindings ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>./bindings.txt |
grep -v '^#' | grep -v '^$' | sed '1d' | awk {'print $1", "$2", "$3", "$4'} | column -t >> ./benchmark1.dat

for i in {1..2} 
do

	mpirun -np 2 --map-by node ~/mpi-benchmarks/src_c/IMB-MPI1 PingPong -msglog 29 2>/dev/null |
	grep -v '^#' | grep -v '^$' | sed '1d' | awk {'print $1", "$2", "$3", "$4'} | column -t >> ./benchmark1.dat

done


