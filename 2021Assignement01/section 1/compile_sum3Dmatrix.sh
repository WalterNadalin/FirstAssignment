#!/bin/bash

module load openmpi-4.1.1+gnu-9.3.0
mpicc sum3Dmatrix.c -o sum3Dmatrix.x
