# Testing permormances

File Benchmark.cc has to be compiled with optimization level -O3 and setting the desired size from command line, with the option-D.
E.g. $ g++ Benchmark.cc -D N_NODES=10000000 -o benchmark -O3
