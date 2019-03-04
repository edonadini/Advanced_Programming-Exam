# Testing performances

The file Benchmark.cc has to be compiled with optimization level -O3.

The desired size is expected to be set from command line with -D option, e.g.:

$ g++ Benchmark.cc -D N_NODES=10000000 -o benchmark -O3.

(The file Benchmark.cc includes BinTree_plain.h, which has the same content as BinTree.h,
but the couts used for debugging purposes were commented in order not to affect the performance of find function).
