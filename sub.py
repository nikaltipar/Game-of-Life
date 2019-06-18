#!/usr/bin/python

from popen2 import popen2
import time

for i in range(4, 13, 4):
  output, input = popen2('qsub')

  job_name = "gameoflife_%d" % i
  processors = "nodes=2:ppn=2"
  command = "mpiexec -np $PBS_NP bin/game-of-life %d 0.5 1" % i

  job_string = """#!/bin/bash
  		  #PBS -N %s
 		  #PBS -q auth
  		  #PBS -j oe
  		  #PBS -l %s
		  module load mpi/mpich3-x86_64
                  cd $PBS_O_WORKDIR
	          export OMP_NUM_THREADS=`cat $PBS_NODEFILE | wc -l`
                  %s""" % (job_name,
		           processors,
                           command)

  input.write(job_string)
  input.close()

  print job_string
  print output.read()

  time.sleep(0.1)

