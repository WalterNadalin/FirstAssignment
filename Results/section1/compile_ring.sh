#!/bin/bash

module load openmpi-4.1.1+gnu-9.3.0
mpicc ring.c -o ring.x
