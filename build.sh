#!/bin/bash

g++ -std=c++17 make_VMagField.cc -O3 -fpic -c
gcc -shared -o libVMagField.so make_VMagField.o
scram tool info google-benchmark
g++ measure.cc -std=c++17 -O3 -I/cvmfs/cms.cern.ch/slc7_amd64_gcc820/external/google-benchmark/1.4.x-bcolbf/include -L/cvmfs/cms.cern.ch/slc7_amd64_gcc820/external/google-benchmark/1.4.x-bcolbf/lib -lbenchmark -lpthread -L. -lVMagField -o mag_benchmark
