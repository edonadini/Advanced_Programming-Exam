# Testing permormances

The file Benchmark.cc has to be compiled with optimization level -O3.
The desired size is expected to be set from command line with the option-D:
$ g++ Benchmark.cc -D N_NODES=10000000 -o benchmark -O3
